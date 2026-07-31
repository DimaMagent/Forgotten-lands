#pragma once
#include <memory>

namespace sl {
	class CollisionCellMap;

	class WorldMap {
	public:
		WorldMap();
	protected:
		std::unique_ptr<sl::CollisionCellMap> CollisionMap;
	};
}