#pragma once
#include <SFML/System/Vector2.hpp>
#include "EntityId.hpp"

namespace sl {
	class Entity;
	class CollisionCellMap;
	class IEntityRegistry;

	class MovementSystem {
	public:
		MovementSystem();

		void onUpdate(float updateTime, const sl::Entity& entity, const sl::CollisionCellMap& collisionCellMap, const IEntityRegistry& entityRegistry);

	private:

		const float MAX_STEP_SIZE = 10.f;

		const float MIN_SPEED_FOR_SUB_STEPPING_ALGORITHM = 30.f;

		const int ON_POSITION_SEARCH_DEPTH = 1;

		std::vector<sl::EntityId> reusableEntityIdsBuffer;

		bool isBlockedOnPosition(const sl::Entity& entity, const sf::Vector2f& testPos,
			const sl::CollisionCellMap& collisionCellMap, const IEntityRegistry& entityRegistry);

		void movingWithCollisionCheck(float updateTime, const sl::Entity& entity,
			const sl::CollisionCellMap& collisionCellMap, const IEntityRegistry& entityRegistry);

		void standartPositionCalculate(const sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos,
			const sl::CollisionCellMap& collisionCellMap, const IEntityRegistry& entityRegistry);

		void subSteppingPositionCalculate(const sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos,
			const sl::CollisionCellMap& collisionCellMap, const IEntityRegistry& entityRegistry);
	};
}