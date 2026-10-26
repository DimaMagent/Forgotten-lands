#include "pch.hpp"
#include "PlayerIntentManager.hpp"
#include "system/Entity.hpp"
#include "system/IEntityRegistry.hpp"
#include "system/WorldMap.hpp"
#include "system/systems/IntentComponent.hpp"

PlayerIntentManager::PlayerIntentManager(
	const sl::IEntityRegistry& entityRegistry,
	sl::LockFreeDelegate<sl::EntityId, sl::Action>& onNewAction,
	sl::LockFreeDelegate<sl::EntityId, const sf::Vector2i& >& onSetMovementDirection,
	sl::LockFreeDelegate<std::optional<sl::EntityId>>& onSetPlayerEntity):
	entityRegistry(entityRegistry)
{
	game_logger = spdlog::get("game");

	tokens.push_back(onNewAction.addFunction([this](sl::EntityId id, sl::Action action)
		{
			if (auto entity = this->entityRegistry.getEntityById(id); entity.has_value()) {
				if (id == playerEntityId) { isIntentsChanged = true; }

				sl::IntentComponent* intentComponent = entity.value().get().getComponent<sl::IntentComponent>();
				if (!intentComponent) { return; }
				intentComponent->addAction(action);
			}
		}));

	tokens.push_back(onSetMovementDirection.addFunction([this](sl::EntityId id, const sf::Vector2i& newDirection)
		{
			if (auto entity = this->entityRegistry.getEntityById(id); entity.has_value()) {
				if (id == playerEntityId) { isIntentsChanged = true; }

				sl::IntentComponent* intentComponent = entity.value().get().getComponent<sl::IntentComponent>();
				if (!intentComponent) { return; }
				intentComponent->setMovementDirection(newDirection);
			}
		}));

	tokens.push_back(onSetPlayerEntity.addFunction([this](std::optional<sl::EntityId> playerEntityId) { onPlayerEntitySet(playerEntityId); }));
}

PlayerIntentManager::~PlayerIntentManager() = default;

void PlayerIntentManager::update(float dt)
{
	timeSinceLastUpdate += std::min(sf::seconds(dt), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;
		if (!isIntentsChanged) { continue; }
	}
}

void PlayerIntentManager::onPlayerEntitySet(std::optional<sl::EntityId> playerEntityId)
{
	this->playerEntityId = playerEntityId;

	if (playerEntityId.has_value()) {
		game_logger->info("Player with id: {} character set in PlayerIntentManager.", playerEntityId.value().ID);
	}
}