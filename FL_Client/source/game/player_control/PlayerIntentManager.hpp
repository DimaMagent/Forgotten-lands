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
		sl::LockFreeDelegate<sl::EntityId, sl::Action>& onNewAction,
		sl::LockFreeDelegate<sl::EntityId, const sf::Vector2i& >& onSetMovementDirection,
		sl::LockFreeDelegate<std::optional<sl::EntityId>>& onSetPlayerEntity
	);

	~PlayerIntentManager();

	void update(float dt);
private:
	std::shared_ptr<spdlog::logger> game_logger;

	const sl::IEntityRegistry& entityRegistry;

	std::optional<sl::EntityId> playerEntityId;

	const sf::Time updateTime = sf::seconds(1.f / 30.f);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	bool isIntentsChanged = false;

	std::vector<std::shared_ptr<sl::DelegateToken>> tokens;

	void onPlayerEntitySet(std::optional<sl::EntityId> playerEntityId);

};