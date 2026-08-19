#include "pch.hpp"
#include "PlayerIntentManager.hpp"
#include "MovementComponent.hpp"
#include "Packer.hpp"
#include "PlayerIntentionsPacket.hpp"
#include "Entity.hpp"

PlayerIntentManager::PlayerIntentManager(
	sl::LockFreeDelegate<sl::net::Action>& onNewAction,
	sl::LockFreeDelegate<const sf::Vector2i& >& onSetMovementDirection,
	sl::LockFreeDelegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntity)
{
	game_logger = spdlog::get("game");

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

void PlayerIntentManager::tick(float dt)
{
	timeSinceLastUpdate += std::min(sf::seconds(dt), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;
		if (!isIntentsChanged) { continue; }

		if (!setMovingDirection()) {
			game_logger->warn("PlayerIntentManager::tick movingDirection cannot change: playerEntity or movementComponent is not valid");
		}

		for (auto action: currentIntentions.actions) {
			actionCheck(action);
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

void PlayerIntentManager::actionCheck(sl::net::Action action)
{
	if (action == sl::net::Action::None) { return; }

	if (action == sl::net::Action::Attack) {
		std::cout << "attack\n";
	}
}
