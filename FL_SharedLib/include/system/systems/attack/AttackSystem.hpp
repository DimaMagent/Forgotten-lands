#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <set>
#include "system/EntityId.hpp"
#include "system/systems/UpdatableSystem.hpp"
#include "system/systems/Action.hpp"

namespace sl {
	class Entity;
	struct AABB;
	class CollisionCellMap;
	class WorldBase;
	class IEntityRegistry;

	//не работает из-за системы очистки actions, поэтому атака не может начаться толком
	class AttackSystem: public sl::UpdatableSystem {
	public:
		AttackSystem(const sl::CollisionCellMap& collisionCellMap, const sl::IEntityRegistry& entityRegistry);
		void update(float updateTime, const sl::Entity& entity) override;
		[[nodiscard]] bool tryMeleeAttack(const sl::Entity& attackingEntity);
		void attackEnd(sl::EntityId attackingEntityId, const sl::IEntityRegistry& entityRegistry);

	private:
		const sl::CollisionCellMap& collisionCellMap;
		const sl::IEntityRegistry& entityRegistry;

		std::vector<sl::EntityId> reusableEntityIdsBuffer;

		std::set<sl::EntityId> temporaryIgnoreList;

		sl::Action triggerAction = sl::Action::Attack;

		[[nodiscard]] bool isAABBinAttackCone(
			sf::Vector2f attackerPos,
			sf::Vector2f lookDir,
			const sl::AABB& targetAABB,
			sf::Vector2f targetPos,
			float attackRadius,
			float maxAngleDegrees);

		RegisterUpdatableSystem();
	};

}