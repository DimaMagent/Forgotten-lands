#pragma once
#include <SFML/System/Time.hpp>
#include "system/handlers/LockFreeDelegate.hpp"
#include "network/packets/Intentions.hpp"
#include "system/EntityId.hpp"


namespace sl {
	class Entity;
	class IEntityRegistry;
}

class PlayerIntentManager {
public:
	PlayerIntentManager(
		const sl::IEntityRegistry& entityRegistry,
		sl::LockFreeDelegate<sl::EntityId, sl::Action, bool>& onNewAction,
		sl::LockFreeDelegate<sl::EntityId, const sf::Vector2i& >& onSetMovementDirection);

	~PlayerIntentManager();

private:
	std::shared_ptr<spdlog::logger> game_logger;

	const sl::IEntityRegistry& entityRegistry;

	std::vector<std::shared_ptr<sl::DelegateToken>> tokens;

};