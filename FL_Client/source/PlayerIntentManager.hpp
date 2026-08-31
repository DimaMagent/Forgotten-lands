#pragma once
#include <SFML/System/Time.hpp>
#include "LockFreeDelegate.hpp"
#include "Intentions.hpp"
#include "EntityId.hpp"


namespace sl {
	class Entity;
	class AttackSystem;
	class WorldBase;
	class IEntityRegistry;
}

class PlayerIntentManager {
public:
	PlayerIntentManager(
		sl::LockFreeDelegate<sl::net::Action>& onNewAction,
		sl::LockFreeDelegate<const sf::Vector2i& >& onSetMovementDirection,
		sl::LockFreeDelegate<std::optional<sl::EntityId>>& onSetPlayerEntity
	);

	~PlayerIntentManager();

	void tick(float dt, const sl::WorldBase& world);
private:
	std::shared_ptr<spdlog::logger> game_logger;

	sl::Intentions currentIntentions;

	std::optional<sl::EntityId> playerEntityId;

	std::unique_ptr<sl::AttackSystem> attackSystem;

	const sf::Time updateTime = sf::seconds(1.f / 30.f);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	bool isIntentsChanged = false;

	void onPlayerEntitySet(std::optional<sl::EntityId> playerEntityId);

	//returns true if direction changed successfully
	[[nodiscard]] bool setMovingDirection(const sl::IEntityRegistry& entityRegistry) const;

	void actionCheck(sl::net::Action action, const sl::WorldBase& world);
};