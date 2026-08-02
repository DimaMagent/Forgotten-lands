#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <cstddef>
#include "Aabb.hpp"

namespace sl {
	struct Cell {
		uint16_t x;
		uint16_t y;
		Cell(float posX, float posY);
		Cell(uint8_t cellNumX, uint8_t cellNumY);

		bool operator==(const Cell& other) const noexcept {
			return x == other.x && y == other.y;
		}
	private:
		float cellSize = 100.f;
	};
}
namespace std {
	template <>
	struct hash<sl::Cell> {
		std::size_t operator()(const sl::Cell& ñ) const noexcept {
			std::size_t h1 = std::hash<int>{}(ñ.x);
			std::size_t h2 = std::hash<int>{}(ñ.y);

			std::size_t seed = h1;
			seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}

namespace sl {
	class Entity;

	//stores <cell, EntityIds>
	class CollisionCellMap {
	public:
		CollisionCellMap() = default;
		void recordEntityToCollisionMap(const sl::Entity& entity, AABB aabb);
		void removeEntityToCollisionMap(uint32_t entityId, AABB aabb);
		std::vector<uint32_t> getEntityIdsToCollisionMap(float posX, float posY) const;
		std::vector<uint32_t> getNearestEntityIdsToPosition(float posX, float posY, uint8_t searchDepth = 1);
	private:
		std::unordered_map<Cell, std::vector<uint32_t>> cellToEntityIds;

		AABB onMapBound(const AABB& aabb) const;
	};
}