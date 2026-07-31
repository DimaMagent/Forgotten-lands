#include "pch.h"
#include "CollisionCellMap.hpp"
#include "Entity.hpp"
#include "CollisionComponent.hpp"
#include <queue>
#include <array>

sl::Cell::Cell(float posX, float posY)
{
	if (posX <= 0) { x = 0; }
	if (posY <= 0) { y = 0; }

	x = static_cast<uint16_t>(posX / 100.f);
	y = static_cast<uint16_t>(posY / 100.f);
}


void sl::CollisionCellMap::recordEntityToCollisionMap(const sl::Entity& entity, float posX, float posY)
{
	if (posX < 0 || posY < 0) { return; }

	if (!entity.hasComponent<sl::CollisionComponent>()) { return; }

	Cell currentCell(posX, posY);

	cellToEntityIds[currentCell].push_back(entity.getGlobalId());
}

void sl::CollisionCellMap::removeEntityToCollisionMap(uint32_t entityId, float posX, float posY)
{
	if (posX < 0 || posY < 0) { return; }

	Cell cell(posX, posY);

	auto it = cellToEntityIds.find(cell);

	if (it == cellToEntityIds.end()) { return; }

	auto removeIt = std::find(it->second.begin(), it->second.end(), entityId);

	if (removeIt == it->second.end()) { return; }

	*removeIt = it->second.back();
	it->second.pop_back();
}

std::vector<uint32_t> sl::CollisionCellMap::getEntityIdsToCollisionMap(float posX, float posY) const
{
	if (posX < 0 || posY < 0) { return  std::vector<uint32_t>(); }

	Cell cell(posX, posY);

	auto it = cellToEntityIds.find(cell);

	if (it == cellToEntityIds.end()) { return  std::vector<uint32_t>(); }

	return it->second;
}

std::vector<uint32_t> sl::CollisionCellMap::getNearestEntityIdsToEntity(float posX, float posY, uint8_t searchDepth)
{
	std::vector<uint32_t> entityIds;
	if (posX < 0.f || posY < 0.f) { return entityIds; }

	Cell centerCell(posX, posY);

	int minX = std::max(0, static_cast<int>(centerCell.x) - searchDepth);
	int maxX = static_cast<int>(centerCell.x) + searchDepth;
	int minY = std::max(0, static_cast<int>(centerCell.y) - searchDepth);
	int maxY = static_cast<int>(centerCell.y) + searchDepth;

	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			if (x < 0 || y < 0) { continue; }

			Cell cell(static_cast<float>(x * 100), static_cast<float>(y * 100));

			auto it = cellToEntityIds.find(cell);
			if (it != cellToEntityIds.end()) {
				entityIds.insert(entityIds.end(), it->second.begin(), it->second.end());
			}
		}
	}

	return entityIds;
}

