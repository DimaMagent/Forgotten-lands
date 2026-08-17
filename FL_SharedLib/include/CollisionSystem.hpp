#pragma once
#include "LockFreeDelegate.hpp"
#include <vector>
#include "SFML/System/Vector2.hpp"

namespace sl {
	class Entity;
	class CollisionCellMap;
	class WorldBase;
	struct EntityStorage;
	enum class CollisionType : uint8_t;

	class CollisionSystem {
	public:
		CollisionSystem(const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);
		void onUpdate(float updateTime);

		sl::LockFreeDelegate<sl::Entity&, sl::Entity&, sl::CollisionType> onCollisionDetected;
	private:
		float updateRate = 1.0f / 10.0f;
		float currentTime = 0.f;

		const int SEARCH_DEPTH = 1;

		std::vector<uint32_t> reusableEntityIdsBuffer;

		const sl::CollisionCellMap& collisionCellMap;
		const WorldBase& world;
	};
}