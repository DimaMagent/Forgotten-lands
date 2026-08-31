#pragma once
#include "SFML/System/Time.hpp"
#include "LockFreeDelegate.hpp"


namespace sl {
	class WorldBase;
	class MovementSystem;
	class CollisionSystem;
	class Entity;

	class SystemUpdater {
	public:

		SystemUpdater();
		virtual ~SystemUpdater();

		void update(float deltaTime, const sl::WorldBase& world);
	protected:
		const sf::Time updateTime = sf::seconds(1.f / 60.f);
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		std::unique_ptr<sl::CollisionSystem> collisionSystem;
		std::unique_ptr<MovementSystem> movementSystem;

		virtual void onUpdate(float deltaTime, const sl::WorldBase& world);

		virtual void onEnitiesUpdate(float deltaTime, const sl::WorldBase& world, const sl::Entity& entity);
	};

}