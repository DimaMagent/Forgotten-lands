#pragma once
#include "LockFreeDelegate.hpp"
#include <vector>
#include "SFML/System/Vector2.hpp"
#include "EntityId.hpp"

namespace sl {
	class Entity;
	class CollisionCellMap;
	class WorldBase;
	struct EntityStorage;
	enum class CollisionType : uint8_t;

	class CollisionSystem {
	public:
		CollisionSystem();
		void onUpdate(float updateTime, const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);

		sl::LockFreeDelegate<const sl::Entity&, const sl::Entity&, sl::CollisionType> onCollisionDetected;
	private:
		float updateRate = 1.0f / 10.0f;
		float currentTime = 0.f;

		const uint8_t SEARCH_DEPTH = 1;

		std::vector<sl::EntityId> reusableEntityIdsBuffer;
	};
}