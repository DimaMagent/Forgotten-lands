#pragma once
#include "LockFreeDelegate.hpp"

namespace sl {
	class Entity;
	class CollisionCellMap;
	class WorldBase;
	struct EntityStorage;
	enum class CollisionType : uint8_t;

	class CollisionSystem {
	public:
		CollisionSystem(const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);
		void onUpdate(sl::Entity& entity, float updateTime);

		sl::LockFreeDelegate<sl::Entity&, sl::Entity&, sl::CollisionType> onCollisionDetected;
	private:
		float updateRate = 1 / 10;
		float currentTime = 0.f;

		const sl::CollisionCellMap& collisionCellMap;
		const sl::WorldBase& world;
	};
}