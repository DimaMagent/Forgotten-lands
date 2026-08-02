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

	x = static_cast<uint16_t>(posX / cellSize);
	y = static_cast<uint16_t>(posY / cellSize);
}

sl::Cell::Cell(uint8_t cellNumX, uint8_t cellNumY)
{
	if (cellNumX < 0) { x = 0; }
	if (cellNumY < 0) { y = 0; }

	x = cellNumX;
	y = cellNumY;
}


void sl::CollisionCellMap::recordEntityToCollisionMap(const sl::Entity& entity, AABB aabb)
{
	if (!entity.hasComponent<sl::CollisionComponent>()) { return; }

	AABB boundedAABB = onMapBound(aabb);

	uint8_t minCellX = static_cast<uint8_t>(boundedAABB.topX / 100.f);
	uint8_t maxCellX = static_cast<uint8_t>(boundedAABB.downX / 100.f);
	uint8_t minCellY = static_cast<uint8_t>(boundedAABB.topY / 100.f);
	uint8_t maxCellY = static_cast<uint8_t>(boundedAABB.downY / 100.f);

	for (uint8_t cellNumX = minCellX; cellNumX <= maxCellX; ++cellNumX) {
		for (uint8_t cellNumY = minCellY; cellNumY <= maxCellY; ++cellNumY) {
			Cell currentCell(cellNumX, cellNumY);
			cellToEntityIds[currentCell].push_back(entity.getGlobalId());
		}
	}
}

void sl::CollisionCellMap::removeEntityToCollisionMap(uint32_t entityId, AABB aabb)
{
	if (aabb.topX < 0 || aabb.topY < 0 || aabb.downX < 0 || aabb.downY < 0) { return; }

	Cell cell(aabb.topX, aabb.topY);
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

std::vector<uint32_t> sl::CollisionCellMap::getNearestEntityIdsToPosition(float posX, float posY, uint8_t searchDepth)
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

sl::AABB sl::CollisionCellMap::onMapBound(const AABB& aabb) const
{
	AABB aabbOut(aabb.topX, aabb.topY, aabb.downX, aabb.downY);
	if (aabb.topX < 0) { aabbOut.topX = 0; }
	if (aabb.topY < 0) { aabbOut.topY = 0; }
	if (aabb.downX < 0) { aabbOut.downX = 0; }
	if (aabb.downY < 0) { aabbOut.downY = 0; }

	return aabbOut;
}

sl::AABB::AABB(float topX, float topY, float downX, float downY)
{
	this->topX = topX;
	this->topY = topY;
	this->downX = downX;
	this->downY = downY;
}
