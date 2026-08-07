#include "pch.h"
#include "WorldMap.hpp"
#include "CollisionCellMap.hpp"

sl::WorldMap::WorldMap()
{
	collisionMap = std::make_unique<sl::CollisionCellMap>();
}

sl::WorldMap::~WorldMap() = default;

void sl::WorldMap::onEntityAdded(const sl::Entity& en)
{
	collisionMap->recordEntityToCollisionMap(en);
}

void sl::WorldMap::onEntityRemoved(const sl::Entity& en)
{
	collisionMap->removeEntityToCollisionMap(en);
}
