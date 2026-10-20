#pragma once
#include "system/systems/UpdatableSystem.hpp"
#include "system/handlers/LockFreeDelegate.hpp"
#include <vector>
#include "SFML/System/Vector2.hpp"
#include "system/EntityId.hpp"

namespace sl {
	class Entity;
	class CollisionCellMap;
	class IEntityRegistry;
	struct EntityStorage;
	enum class CollisionType : uint8_t;

	class CollisionSystem : public sl::UpdatableSystem {
	public:
		CollisionSystem(const sl::CollisionCellMap& collisionCellMap, const sl::IEntityRegistry& entityRegistry);
		void update(float updateTime, const sl::Entity& entity);

		sl::LockFreeDelegate<const sl::Entity&, const sl::Entity&, sl::CollisionType> onCollisionDetected;

	private:
		float updateRate = 1.0f / 10.0f;
		float currentTime = 0.f;

		const uint8_t SEARCH_DEPTH = 1;

		std::vector<sl::EntityId> reusableEntityIdsBuffer;

		const sl::CollisionCellMap& collisionCellMap;

		const sl::IEntityRegistry& entityRegistry;

		RegisterUpdatableSystem();
	};
}