#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_map>

namespace sl {
	using cellIndex = uint16_t;


	//stores (x, y) cell's index
	struct Cell {
		cellIndex x;
		cellIndex y;
		Cell(float posX, float posY);
		Cell(cellIndex cellNumX, cellIndex cellNumY);
		static float getCellSize() { return cellSize; }

		bool operator==(const Cell& other) const noexcept {
			return x == other.x && y == other.y;
		}
	private:
		static constexpr float cellSize = 100.f;
	};
}
namespace std {
	template <>
	struct hash<sl::Cell> {
		std::size_t operator()(const sl::Cell& c) const noexcept {
			std::size_t h1 = std::hash<int>{}(c.x);
			std::size_t h2 = std::hash<int>{}(c.y);

			std::size_t seed = h1;
			seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}