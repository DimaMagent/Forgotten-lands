#include "pch.hpp"
#include "PlayerIntentManager.hpp"
#include "system/Entity.hpp"
#include "system/IEntityRegistry.hpp"
#include "system/WorldMap.hpp"
#include "system/systems/IntentComponent.hpp"

PlayerIntentManager::PlayerIntentManager(
	const sl::IEntityRegistry& entityRegistry,
	sl::LockFreeDelegate<sl::EntityId, sl::Action, bool>& onNewAction,
	sl::LockFreeDelegate<sl::EntityId, const sf::Vector2i& >& onSetMovementDirection):
	entityRegistry(entityRegistry)
{
	game_logger = spdlog::get("game");

	tokens.push_back(onNewAction.addFunction([this](sl::EntityId id, sl::Action action, bool ActionState)
		{
			if (auto entity = this->entityRegistry.getEntityById(id); entity.has_value()) {

				sl::IntentComponent* intentComponent = entity.value().get().getComponent<sl::IntentComponent>();
				if (!intentComponent) { return; }
				intentComponent->setAction(action, ActionState);
			}
		}));

	tokens.push_back(onSetMovementDirection.addFunction([this](sl::EntityId id, const sf::Vector2i& newDirection)
		{
			if (auto entity = this->entityRegistry.getEntityById(id); entity.has_value()) {

				sl::IntentComponent* intentComponent = entity.value().get().getComponent<sl::IntentComponent>();
				if (!intentComponent) { return; }
				intentComponent->setMovementDirection(newDirection);
			}
		}));
}

PlayerIntentManager::~PlayerIntentManager() = default;