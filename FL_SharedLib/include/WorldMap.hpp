#pragma once
#include <memory>

namespace sl {
	class CollisionCellMap;
	class Entity;

	class WorldMap {
	public:
		WorldMap();
		~WorldMap();
		sl::CollisionCellMap& getCollisionMap() const { return *collisionMap; }
		void onEntityAdded(const sl::Entity& en);
		void onEntityRemoved(const sl::Entity& en);
	protected:
		std::unique_ptr<sl::CollisionCellMap> collisionMap;
	};
}