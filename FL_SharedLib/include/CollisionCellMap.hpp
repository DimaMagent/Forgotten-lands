#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace sl {
	class Entity;

	struct Cell {
		uint16_t x;
		uint16_t y;
		Cell(float posX, float posY);
	};

	//stores <cell, EntityIds>
	class CollisionCellMap {
	public:
		CollisionCellMap() = default;
		void recordEntityToCollisionMap(const sl::Entity& entity, float posX, float posY);
		void removeEntityToCollisionMap(uint32_t entityId, float posX, float posY);
		std::vector<uint32_t> getEntityIdsToCollisionMap(float posX, float posY) const;
		std::vector<uint32_t> getNearestEntityIdsToEntity(float posX, float posY, uint8_t searchDepth = 1);
	private:
		float cellSize = 100.f;
		std::unordered_map<Cell, std::vector<uint32_t>> cellToEntityIds;
	};
}