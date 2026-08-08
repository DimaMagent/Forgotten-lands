#pragma once
#include <memory>
#include "Cell.hpp"

namespace sl {
	class CollisionCellMap;
	class Entity;

	class WorldMap {
	public:
		WorldMap(cellIndex mapSizeX = 10, cellIndex mapSizeY = 10);
		~WorldMap();
		sl::CollisionCellMap& getCollisionMap() const { return *collisionMap; }
		void onEntityAdded(const sl::Entity& en);
		void onEntityRemoved(const sl::Entity& en);
	protected:
		cellIndex mapBoundsX;
		cellIndex mapBoundsY;
		std::unique_ptr<sl::CollisionCellMap> collisionMap;
	};
}