#pragma once
#include <SFML/System/Vector2.hpp>

namespace sl {
	class Entity;
	class CollisionCellMap;
	class WorldBase;

	class MovementSystem {
	public:
		MovementSystem();

		void onUpdate(float updateTime, sl::Entity& entity, const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);

	private:

		const float MAX_STEP_SIZE = 10.f;

		const float MIN_SPEED_FOR_SUB_STEPPING_ALGORITHM = 30.f;

		const int ON_POSITION_SEARCH_DEPTH = 1;

		std::vector<uint32_t> reusableEntityIdsBuffer;

		bool isBlockedOnPosition(sl::Entity& entity, const sf::Vector2f& testPos,
			const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);

		void movingWithCollisionCheck(float updateTime, sl::Entity& entity,
			const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);

		void standartPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos,
			const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);

		void subSteppingPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos,
			const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);
	};
}