#include "pch.h"
#include "system/grids/WorldGridsManager.hpp"
#include "system/grids/CollisionGrid.hpp"
#include "system/grids/SpatialGrid.hpp"
#include "system/IEntityRegistry.hpp"

sl::WorldGridsManager::WorldGridsManager(const IEntityRegistry& entityRegistry,
	sl::LockFreeDelegate<sl::EntityId>& OnEntityAdded,
	sl::LockFreeDelegate<sl::EntityId>& OnEntityRemoved)
{
	collisionGrid = std::make_unique<sl::CollisionGrid>(entityRegistry, OnEntityAdded, OnEntityRemoved);
	spatialGrid = std::make_unique<sl::SpatialGrid>(entityRegistry, OnEntityAdded, OnEntityRemoved);
}

sl::WorldGridsManager::~WorldGridsManager() = default;

const std::optional<std::reference_wrapper<sl::CollisionGrid>> sl::WorldGridsManager::getCollisionMap() const {
	if (!collisionGrid) { return {}; }

	return *collisionGrid;
}

const std::optional<std::reference_wrapper<sl::SpatialGrid>> sl::WorldGridsManager::getspatialGrid() const {
	if (!spatialGrid) { return {}; }

	return *spatialGrid;
}