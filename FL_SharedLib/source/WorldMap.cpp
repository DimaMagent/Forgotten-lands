#include "pch.h"
#include "WorldMap.hpp"
#include "CollisionCellMap.hpp"

sl::WorldMap::WorldMap(cellIndex mapSizeX, cellIndex mapSizeY) :
	mapBoundsX(mapSizeX), mapBoundsY(mapSizeY)
{
	collisionMap = std::make_unique<sl::CollisionCellMap>(mapSizeX, mapSizeY);
}

sl::WorldMap::~WorldMap() = default;

void sl::WorldMap::onEntityAdded(const sl::Entity& en)
{
	collisionMap->recordEntityToCollisionMap(en);
}

void sl::WorldMap::onEntityRemoved(const sl::Entity& en)
{
	collisionMap->removeEntityFromCollisionMap(en);
}
