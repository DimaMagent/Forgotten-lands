#pragma once
#include <SFML/System/Time.hpp>
#include "LockFreeDelegate.hpp"
#include "Intentions.hpp"


namespace sl {
	class Entity;
}

class PlayerIntentManager {
public:
	PlayerIntentManager(
		sl::LockFreeDelegate<sl::net::Action>& onNewAction,
		sl::LockFreeDelegate<const sf::Vector2i& >& onSetMovementDirection,
		sl::LockFreeDelegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntity
	);

	~PlayerIntentManager();

	void tick(float dt);
private:
	std::shared_ptr<spdlog::logger> game_logger;

	sl::Intentions currentIntentions;

	std::weak_ptr<sl::Entity> playerEntity;

	const sf::Time updateTime = sf::seconds(1.f / 30.f);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	bool isIntentsChanged = false;

	void onPlayerEntitySet(std::weak_ptr<sl::Entity> playerEntity);

	//returns true if direction changed successfully
	[[nodiscard]] bool setMovingDirection();

	void actionCheck(sl::net::Action action);
};