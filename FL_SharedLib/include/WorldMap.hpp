#pragma once
#include <memory>
#include "Cell.hpp"

namespace sl {
	class CollisionCellMap;
	class Entity;
	class WorldBase;

	class WorldMap {
	public:
		WorldMap(const WorldBase& world, cellIndex mapSizeX = 10, cellIndex mapSizeY = 10);
		~WorldMap();
		const std::optional<std::reference_wrapper<sl::CollisionCellMap>> getCollisionMap() const;
		void onEntityAdded(const sl::Entity& en);
		void onEntityRemoved(const sl::Entity& en);
	protected:
		cellIndex mapBoundsX;
		cellIndex mapBoundsY;
		std::unique_ptr<sl::CollisionCellMap> collisionMap;
	};
}