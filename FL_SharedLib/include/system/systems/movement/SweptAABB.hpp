#pragma once
#include "SFML/System/Vector2.hpp"
#include "system/systems/UpdatableSystem.hpp"

namespace sl {
	class Entity;
	class CollisionCellMap;
	class IEntityRegistry;
	class AABB;

	class SweptAABB : public sl::UpdatableSystem {
	public:
		SweptAABB(const sl::CollisionCellMap& collisionCellMap, const IEntityRegistry& entityRegistry);

		void update(float updateTime, const sl::Entity& entity) override;

	private:
		struct SweptResult {
			sf::Vector2f collisionPosition;
			sf::Vector2f remainingMovement;

			float collidedTime;
			bool isCollided;
		};

		const sl::CollisionCellMap& collisionCellMap;

		const IEntityRegistry& entityRegistry;

		SweptResult movementCheck(const sf::Vector2f& movementStartPoint, const sf::Vector2f& movementEndPoint,
			const sf::Vector2f& blockerPos, const sl::AABB& movingAABB, const sl::AABB& blockerAABB);

		RegisterUpdatableSystem();
	};

}