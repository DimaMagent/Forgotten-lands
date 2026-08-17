#pragma once
#include <SFML/System/Vector2.hpp>

namespace sl {
	class Entity;
	class CollisionSystem;

	class MovementSystem {
	public:
		MovementSystem(CollisionSystem& collisionSystem);

		void onUpdate(sl::Entity& entity, float updateTime) const;
	private:
		CollisionSystem& collisionSystem; //temp

		const float MAX_STEP_SIZE = 10.f;
		const float MIN_SPEED_FOR_SUB_STEPPING_ALGORITHM = 30.f;

		void movingWithCollisionCheck(sl::Entity& entity, float updateTime) const;
		void standartPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos) const;
		void subSteppingPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos) const;
	};
}