#include "pch.hpp"
#include "PlayerIntentManager.hpp"
#include "MovementComponent.hpp"
#include "Packer.hpp"
#include "PlayerIntentionsPacket.hpp"
#include "Entity.hpp"
#include "AttackSystem.hpp"
#include "IEntityRegistry.hpp"
#include "WorldBase.hpp"

PlayerIntentManager::PlayerIntentManager(
	sl::LockFreeDelegate<sl::net::Action>& onNewAction,
	sl::LockFreeDelegate<const sf::Vector2i& >& onSetMovementDirection,
	sl::LockFreeDelegate<std::optional<sl::EntityId>>& onSetPlayerEntity)
{
	game_logger = spdlog::get("game");

	attackSystem = std::make_unique<sl::AttackSystem>();

	onNewAction.addFunction([this](sl::net::Action action) 
		{
			currentIntentions.actions.push_back(action);
			isIntentsChanged = true;
		});

	onSetMovementDirection.addFunction([this](const sf::Vector2i& newDirection) 
		{
			currentIntentions.movementDirectionIntentions = newDirection;
			isIntentsChanged = true;
		});

	onSetPlayerEntity.addFunction([this](std::optional<sl::EntityId> playerEntityId) { onPlayerEntitySet(playerEntityId); });
}

PlayerIntentManager::~PlayerIntentManager() = default;

void PlayerIntentManager::tick(float dt, const sl::WorldBase& world)
{
	timeSinceLastUpdate += std::min(sf::seconds(dt), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;
		if (!isIntentsChanged) { continue; }
		
		if (!setMovingDirection(world)) {
			#ifdef DEBUG
			game_logger->warn("PlayerIntentManager::tick movingDirection cannot change: playerEntity or movementComponent is not valid");
			#endif
		}

		for (auto action: currentIntentions.actions) {
			actionCheck(action, world);
		}

		Packer::send<sl::net::PlayerIntentionsPacket>(currentIntentions);

		currentIntentions.actions.clear();
		isIntentsChanged = false;
	}
}

void PlayerIntentManager::onPlayerEntitySet(std::optional<sl::EntityId> playerEntityId)
{
	this->playerEntityId = playerEntityId;

	if (playerEntityId.has_value()) {
		game_logger->info("Player with id: {} character set in PlayerIntentManager.", playerEntityId.value().ID);
	}
}

bool PlayerIntentManager::setMovingDirection(const sl::IEntityRegistry& entityRegistry) const
{
	if (!playerEntityId.has_value()) { return false; }

	auto player = entityRegistry.getEntityById(playerEntityId.value());
	if (!player.has_value()) { return false; }

	sl::MovementComponent* movComp = player.value().get().getComponent<sl::MovementComponent>();

	if (!movComp) { return false; }

	if (movComp->getVelocityDirection() != currentIntentions.movementDirectionIntentions) {
		movComp->setVelocityDirection(currentIntentions.movementDirectionIntentions);
	}

	return true;
}

void PlayerIntentManager::actionCheck(sl::net::Action action, const sl::WorldBase& world)
{

	if (!playerEntityId.has_value())
	{
		#ifdef DEBUG
		game_logger->warn("PlayerIntentManager::actionCheck cannot call any action: playerEntity is not set");
		#endif
		return; 
	}

	auto playerEn = world.getEntityById(playerEntityId.value());

	if (!playerEn.has_value())
	{
		#ifdef DEBUG
		game_logger->warn("PlayerIntentManager::actionCheck cannot call any action: playerEntity is not valid");
		#endif
		return;
	}

	if (action == sl::net::Action::None) { return; }

	if (action == sl::net::Action::Attack) {

		if (!attackSystem) {
			#ifdef DEBUG
			game_logger->error("PlayerIntentManager::actionCheck cannot call tryMeleeAttack: attackSystem is not valid");
			#endif
			return;
		}

		bool isSucess = attackSystem->tryMeleeAttack(*playerEn, world);

		if (!isSucess) {
			#ifdef DEBUG
			game_logger->warn("PlayerIntentManager::actionCheck tryMeleeAttack was unsuccessful");
			#endif
		}
	}
}
