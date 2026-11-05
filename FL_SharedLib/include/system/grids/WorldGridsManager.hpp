#pragma once
#include <memory>
#include "system/Cell.hpp"
#include "system/handlers/LockFreeDelegate.hpp"
#include "system/EntityId.hpp"

namespace sl {
	class CollisionGrid;
	class Entity;
	class IEntityRegistry;
	class SpatialGrid;

	class WorldGridsManager {
	public:
		WorldGridsManager(const IEntityRegistry& entityRegistry,
			sl::LockFreeDelegate<sl::EntityId>& OnEntityAdded,
			sl::LockFreeDelegate<sl::EntityId>& OnEntityRemoved
		);
		~WorldGridsManager();
		const std::optional<std::reference_wrapper<sl::CollisionGrid>> getCollisionMap() const;
		const std::optional<std::reference_wrapper<sl::SpatialGrid>> getspatialGrid() const;
	protected:
		std::unique_ptr<sl::CollisionGrid> collisionGrid;
		std::unique_ptr<sl::SpatialGrid> spatialGrid;
	};
}