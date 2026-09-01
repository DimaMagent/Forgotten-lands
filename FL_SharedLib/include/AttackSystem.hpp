#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <set>
#include "EntityId.hpp"

namespace sl {
	class Entity;
	struct AABB;
	class CollisionCellMap;
	class WorldBase;

	class AttackSystem {
	public:
		AttackSystem();
		[[nodiscard]] bool tryMeleeAttack(const sl::Entity& attackingEntity, const sl::CollisionCellMap& collisionCellMap, const sl::IEntityRegistry& entityRegistry);
		void attackEnd(sl::EntityId attackingEntityId, const sl::IEntityRegistry& entityRegistry);

	private:
		std::vector<sl::EntityId> reusableEntityIdsBuffer;

		std::set<sl::EntityId> temporaryIgnoreList;

		[[nodiscard]] bool isAABBinAttackCone(
			sf::Vector2f attackerPos,
			sf::Vector2f lookDir,
			const sl::AABB& targetAABB,
			sf::Vector2f targetPos,
			float attackRadius,
			float maxAngleDegrees);


	};

}