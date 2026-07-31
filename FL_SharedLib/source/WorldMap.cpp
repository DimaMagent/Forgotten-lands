#include "pch.h"
#include "WorldMap.hpp"
#include "CollisionCellMap.hpp"

sl::WorldMap::WorldMap()
{
	CollisionMap = std::make_unique<sl::CollisionCellMap>();
}
