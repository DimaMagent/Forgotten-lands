#include "pch.hpp"
#include "PlayerIntentManager.hpp"
#include "MovementComponent.hpp"
#include "Packer.hpp"
#include "PlayerIntentionsPacket.hpp"
#include "Entity.hpp"
#include "AttackSystem.hpp"

PlayerIntentManager::PlayerIntentManager(
	sl::LockFreeDelegate<sl::net::Action>& onNewAction,
	sl::LockFreeDelegate<const sf::Vector2i& >& onSetMovementDirection,
	sl::LockFreeDelegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntity)
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

	onSetPlayerEntity.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) { onPlayerEntitySet(playerEntity); });
}

PlayerIntentManager::~PlayerIntentManager() = default;

void PlayerIntentManager::tick(float dt, const sl::WorldBase& world)
{
	timeSinceLastUpdate += std::min(sf::seconds(dt), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;
		if (!isIntentsChanged) { continue; }
		
		if (!setMovingDirection()) {
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

void PlayerIntentManager::onPlayerEntitySet(std::weak_ptr<sl::Entity> playerEntity)
{
	if (playerEntity.expired()) {
		game_logger->warn("Received expired player character reference.");
		return;
	}
	game_logger->info("Player character set in PlayerIntentManager.");
	this->playerEntity = playerEntity;
}

bool PlayerIntentManager::setMovingDirection()
{
	auto player = playerEntity.lock();
	if (!player) { return false; }

	sl::MovementComponent* movComp = player->getComponent<sl::MovementComponent>();

	if (!movComp) { return false; }

	if (movComp->getVelocityDirection() != currentIntentions.movementDirectionIntentions) {
		movComp->setVelocityDirection(currentIntentions.movementDirectionIntentions);
	}

	return true;
}

void PlayerIntentManager::actionCheck(sl::net::Action action, const sl::WorldBase& world)
{
	if (!attackSystem) {
		#ifdef DEBUG
		game_logger->error("PlayerIntentManager::actionCheck cannot call tryMeleeAttack: attackSystem is not valid");
		#endif
		return;
	}

	auto playerEn = playerEntity.lock();

	if (!playerEn)
	{
		#ifdef DEBUG
		game_logger->warn("PlayerIntentManager::actionCheck cannot call tryMeleeAttack: playerEntity is not valid");
		#endif
		return;
	}

	if (action == sl::net::Action::None) 
	{ 
		attackSystem->attackEnd(*playerEn);
		return; 
	}

	if (action == sl::net::Action::Attack) {


		bool isSucess = attackSystem->tryMeleeAttack(*playerEn, world);

		if (!isSucess) {
			#ifdef DEBUG
			game_logger->warn("PlayerIntentManager::actionCheck tryMeleeAttack was unsuccessful");
			#endif
		}
	}
}
