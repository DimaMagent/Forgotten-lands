#pragma once
#include <SFML/System/Vector2.hpp>
namespace sl {
	class Entity;
	struct AABB;
	class CollisionCellMap;
	class WorldBase;

	class AttackSystem {
	public:
		AttackSystem(const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);
		[[nodiscard]] bool tryMeleeAttack(sl::Entity& attackingEntity);

	private:
		const sl::CollisionCellMap& collisionCellMap;
		const WorldBase& world;

		std::vector<uint32_t> reusableEntityIdsBuffer;

		[[nodiscard]] bool isAABBinAttackCone(
			sf::Vector2f attackerPos,
			sf::Vector2f lookDir,
			const sl::AABB& targetAABB,
			sf::Vector2f targetPos,
			float attackRadius,
			float maxAngleDegrees);


	};

}