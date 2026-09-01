#include "pch.h"
#include "WorldMap.hpp"
#include "CollisionCellMap.hpp"
#include "WorldBase.hpp"

sl::WorldMap::WorldMap(const WorldBase& world, cellIndex mapSizeX, cellIndex mapSizeY) :
	mapBoundsX(mapSizeX), mapBoundsY(mapSizeY)
{
	collisionMap = std::make_unique<sl::CollisionCellMap>(world, mapSizeX, mapSizeY);
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

const std::optional<std::reference_wrapper<sl::CollisionCellMap>> sl::WorldMap::getCollisionMap() const {
	if (!collisionMap) { return {}; }

	return *collisionMap;
}