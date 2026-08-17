#pragma once
#include <SFML/System/Vector2.hpp>

namespace sl {
	class Entity;
	class CollisionCellMap;
	class WorldBase;

	class MovementSystem {
	public:
		MovementSystem(const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);

		void onUpdate(sl::Entity& entity, float updateTime);

	private:
		const sl::CollisionCellMap& collisionCellMap;

		const WorldBase& world;

		const float MAX_STEP_SIZE = 10.f;

		const float MIN_SPEED_FOR_SUB_STEPPING_ALGORITHM = 30.f;

		const int ON_POSITION_SEARCH_DEPTH = 1;

		std::vector<uint32_t> reusableEntityIdsBuffer;

		bool isBlockedOnPosition(sl::Entity& entity, const sf::Vector2f& testPos);

		void movingWithCollisionCheck(sl::Entity& entity, float updateTime);

		void standartPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos);

		void subSteppingPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos);
	};
}