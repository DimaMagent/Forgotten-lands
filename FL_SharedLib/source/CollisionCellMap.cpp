#include "pch.h"
#include "CollisionCellMap.hpp"
#include "Entity.hpp"
#include "CollisionComponent.hpp"
#include "TransformComponent.hpp"
#include <queue>
#include <array>
#include "Utils.hpp"


void sl::CollisionCellMap::recordEntityToCollisionMap(const sl::Entity& entity)
{
	sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
	if (!colisComp) { return; }

	sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
	if (!transComp) { return; }

	sf::Vector2f position = transComp->getPosition();

	bool isSuccess = occupiedCellsAdd(entity.getGlobalId(), position, *colisComp);

	if (!isSuccess) { return; }

	uint64_t token = transComp->onCellChanged.addFunction([this, &entity](sf::Vector2f pos) {
		adjustingEntityOnMap(entity, pos);
		});

	entityIdToDelegateToken.try_emplace(entity.getGlobalId(), token);

}

void sl::CollisionCellMap::removeEntityToCollisionMap(const sl::Entity& entity)
{

	bool isSuccess = occupiedCellsRemove(entity);

	if (!isSuccess) { return; }

	sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
	if (!transComp) { return; }

	auto tokenIt = entityIdToDelegateToken.find(entity.getGlobalId());
	if (tokenIt == entityIdToDelegateToken.end()) { return; }

	transComp->onCellChanged.removeFunction(tokenIt->second);
	entityIdToDelegateToken.erase(tokenIt);
}

std::vector<uint32_t> sl::CollisionCellMap::getEntityIdsToCollisionMap(sf::Vector2f pos) const
{
	if (pos.x < 0 || pos.y < 0) { return  std::vector<uint32_t>(); }

	Cell cell(pos.x, pos.y);
	auto it = cellToEntityIds.find(cell);

	if (it == cellToEntityIds.end()) { return  std::vector<uint32_t>(); }

	return it->second;
}

std::vector<uint32_t> sl::CollisionCellMap::getNearestEntityIdsToPosition(sf::Vector2f pos, uint8_t searchDepth) const
{
	std::vector<uint32_t> entityIds;

	if (pos.x < 0.f || pos.y < 0.f) { return entityIds; }
	Cell centerCell(pos.x, pos.y);

	int minX = std::max(0, static_cast<int>(centerCell.x) - searchDepth);
	int maxX = static_cast<int>(centerCell.x) + searchDepth;
	int minY = std::max(0, static_cast<int>(centerCell.y) - searchDepth);
	int maxY = static_cast<int>(centerCell.y) + searchDepth;

	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			if (x < 0 || y < 0) { continue; }

			Cell cell(static_cast<float>(x * Cell::getCellSize()), static_cast<float>(y * Cell::getCellSize()));

			auto it = cellToEntityIds.find(cell);
			if (it != cellToEntityIds.end()) {
				entityIds.insert(entityIds.end(), it->second.begin(), it->second.end());
			}
		}
	}

	return entityIds;
}

std::vector<uint32_t> sl::CollisionCellMap::getNearestEntityIdsToEntity(AABB aabb, sf::Vector2f pos, uint8_t searchDepth) const
{
	std::vector<uint32_t> entityIds;

	if (!aabb.exists()) { return entityIds; }

	if (!onMapBound(aabb, pos)) { return entityIds; }

	Cell minCell(aabb.topX + pos.x, aabb.topY + pos.y);
	Cell maxCell(aabb.downX + pos.x, aabb.downY + pos.y);

	//TODO: Implement search depth logic for nearest entity IDs to entity
}

bool sl::CollisionCellMap::onMapBound(const AABB& aabb, sf::Vector2f pos) const
{
	if (aabb.topX + pos.x < 0 || aabb.topY + pos.y < 0 || aabb.downX + pos.x < 0 || aabb.downY + pos.y < 0) { return false; }

	return true;
}

bool sl::CollisionCellMap::adjustingEntityOnMap(const sl::Entity& entity, sf::Vector2f pos)
{
	bool isSuccess = occupiedCellsRemove(entity);

	if (!isSuccess) { return false; }

	sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
	if (!colisComp) { return false; }

	return occupiedCellsAdd(entity.getGlobalId(), pos, *colisComp);
}

bool sl::CollisionCellMap::occupiedCellsAdd(uint32_t EntityId, sf::Vector2f pos, sl::CollisionComponent& colisComp)
{
	AABB aabb = colisComp.getAABB();

	if (!aabb.exists()) { return false; }

	if (!onMapBound(aabb, pos)) { return false; }

	cellIndex minCellX = static_cast<cellIndex>((aabb.topX + pos.x) / Cell::getCellSize());
	cellIndex maxCellX = static_cast<cellIndex>((aabb.downX + pos.x) / Cell::getCellSize());
	cellIndex minCellY = static_cast<cellIndex>((aabb.topY + pos.y) / Cell::getCellSize());
	cellIndex maxCellY = static_cast<cellIndex>((aabb.downY + pos.y) / Cell::getCellSize());

	for (cellIndex cellNumX = minCellX; cellNumX <= maxCellX; ++cellNumX) {
		for (cellIndex cellNumY = minCellY; cellNumY <= maxCellY; ++cellNumY) {
			Cell currentCell(cellNumX, cellNumY);
			cellToEntityIds[currentCell].push_back(EntityId);
			colisComp.addOccupiedCell(currentCell);
		}
	}
	return true;
}

bool sl::CollisionCellMap::occupiedCellsRemove(const sl::Entity& entity)
{
	sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
	if (!colisComp) { return false; }

	std::vector<sl::Cell> occupiedCells = colisComp->getOccupiedCells();

	for (auto& cell : occupiedCells) {
		auto it = cellToEntityIds.find(cell);
		if (it == cellToEntityIds.end()) { continue; }

		auto itEntity = std::find(it->second.begin(), it->second.end(), entity.getGlobalId());
		if (itEntity == it->second.end()) { continue; }

		sl::swapPopDelete(it->second, std::distance(it->second.begin(), itEntity));
	}

	colisComp->occupiedCellsClear();

	return true;
}


