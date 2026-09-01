#include "pch.h"
#include "CollisionCellMap.hpp"
#include "Entity.hpp"
#include "CollisionComponent.hpp"
#include "TransformComponent.hpp"
#include "IEntityRegistry.hpp"
#include <queue>
#include <array>
#include <cmath>
#include "Utils.hpp"


sl::CollisionCellMap::CollisionCellMap(const IEntityRegistry& entityRegistry, cellIndex mapSizeX, cellIndex mapSizeY):
	entityRegistry(entityRegistry)
{
	for (cellIndex x = 0; x < mapSizeX; ++x) {
		for (cellIndex y = 0; y < mapSizeY; ++y) {
			Cell cell(x, y);
			cellToEntityIds.try_emplace(cell, std::vector<sl::EntityId>());
		}
	}
}

void sl::CollisionCellMap::recordEntityToCollisionMap(const sl::Entity& entity)
{
	sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
	if (!colisComp) { return; }

	sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
	if (!transComp) { return; }

	sf::Vector2f position = transComp->getPosition();

	bool isSuccess = occupiedCellsAdd(entity.getId(), position);

	if (!isSuccess) { return; }

	sl::EntityId entityId = entity.getId();

	uint64_t token = transComp->onCellChanged.addFunction([this, entityId](sf::Vector2f pos) {
		if (!adjustingEntityOnMap(entityId, pos)) {
			std::cout << "sl::CollisionCellMap::recordEntityToCollisionMap: adjustingEntityOnMap failed\n";
		}
		
		});

	entityIdToDelegateToken.try_emplace(entity.getId(), token);

}

void sl::CollisionCellMap::removeEntityFromCollisionMap(const sl::Entity& entity)
{

	bool isSuccess = occupiedCellsRemove(entity.getId());

	if (!isSuccess) { return; }

	sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
	if (!transComp) { return; }

	auto tokenIt = entityIdToDelegateToken.find(entity.getId());
	if (tokenIt == entityIdToDelegateToken.end()) { return; }

	transComp->onCellChanged.removeFunction(tokenIt->second);
	entityIdToDelegateToken.erase(tokenIt);
}

std::vector<sl::EntityId> sl::CollisionCellMap::getEntityIdsToCollisionMap(sf::Vector2f pos) const
{
	if (pos.x < 0 || pos.y < 0) { return  std::vector<sl::EntityId>(); }

	Cell cell(pos.x, pos.y);
	auto it = cellToEntityIds.find(cell);

	if (it == cellToEntityIds.end()) { return  std::vector<sl::EntityId>(); }

	return it->second;
}

bool sl::CollisionCellMap::getNearestEntityIdsToPosition(sf::Vector2f pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth) const
{
	if (pos.x < 0.f || pos.y < 0.f) { return false; }
	Cell centerCell(pos.x, pos.y);

	int minX = std::max(0, static_cast<int>(centerCell.x) - searchDepth);
	int maxX = static_cast<int>(centerCell.x) + searchDepth;
	int minY = std::max(0, static_cast<int>(centerCell.y) - searchDepth);
	int maxY = static_cast<int>(centerCell.y) + searchDepth;

	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			if (x < 0 || y < 0) { continue; }

			Cell cell(static_cast<cellIndex>(x), static_cast<cellIndex>(y));

			auto it = cellToEntityIds.find(cell);
			if (it != cellToEntityIds.end()) {
				entityIdsOut.insert(entityIdsOut.end(), it->second.begin(), it->second.end());
			}
		}
	}

	return true;
}

bool sl::CollisionCellMap::getNearestEntityIdsToEntity(const AABB& aabb, sf::Vector2f pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth) const
{
	if (!aabb.exists()) { return false; }

	if (!onMapBound(aabb, pos)) { return false; }

	Cell minCell(aabb.minX + pos.x, aabb.minY + pos.y);
	Cell maxCell(aabb.maxX + pos.x, aabb.maxY + pos.y);
		
	int minX = std::max(0, static_cast<int>(minCell.x) - searchDepth);
	int maxX = static_cast<int>(maxCell.x) + searchDepth;
	int minY = std::max(0, static_cast<int>(minCell.y) - searchDepth);
	int maxY = static_cast<int>(maxCell.y) + searchDepth;

	for (int x = minX; x <= maxX; ++x) {
		for (int y = minY; y <= maxY; ++y) {
			if (x < 0 || y < 0) { continue; }

			Cell cell(static_cast<cellIndex>(x), static_cast<cellIndex>(y));

			auto it = cellToEntityIds.find(cell);
			if (it != cellToEntityIds.end()) {
				entityIdsOut.insert(entityIdsOut.end(), it->second.begin(), it->second.end());
			}
		}
	}

	return true;
}

uint8_t sl::CollisionCellMap::getSearchDepthByDistance(float distance)
{
	return static_cast<uint8_t>(ceil(distance / Cell::getCellSize()));
}

bool sl::CollisionCellMap::onMapBound(const AABB& aabb, sf::Vector2f pos) const
{
	if (aabb.minX + pos.x < 0 || aabb.minY + pos.y < 0 || aabb.maxX + pos.x < 0 || aabb.maxY + pos.y < 0) { return false; }

	return true;
}

bool sl::CollisionCellMap::adjustingEntityOnMap(sl::EntityId entityId, sf::Vector2f pos)
{
	bool isSuccess = occupiedCellsRemove(entityId);

	if (!isSuccess) { return false; }

	return occupiedCellsAdd(entityId, pos);
}

bool sl::CollisionCellMap::occupiedCellsAdd(sl::EntityId entityId, sf::Vector2f pos)
{
	auto entityOpt = entityRegistry.getEntityById(entityId);

	if (!entityOpt.has_value()) { return false; }

	const sl::Entity& entity = entityOpt.value().get();

	sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
	if (!colisComp) { return false; }

	AABB aabb = colisComp->getAABB();

	if (!aabb.exists()) { return false; }

	if (!onMapBound(aabb, pos)) { return false; }

	cellIndex minCellX = static_cast<cellIndex>((aabb.minX + pos.x) / Cell::getCellSize());
	cellIndex maxCellX = static_cast<cellIndex>((aabb.maxX + pos.x) / Cell::getCellSize());
	cellIndex minCellY = static_cast<cellIndex>((aabb.minY + pos.y) / Cell::getCellSize());
	cellIndex maxCellY = static_cast<cellIndex>((aabb.maxY + pos.y) / Cell::getCellSize());

	for (cellIndex cellNumX = minCellX; cellNumX <= maxCellX; ++cellNumX) {
		for (cellIndex cellNumY = minCellY; cellNumY <= maxCellY; ++cellNumY) {
			Cell currentCell(cellNumX, cellNumY);
			cellToEntityIds[currentCell].push_back(entityId);
			colisComp->addOccupiedCell(currentCell);
		}
	}
	return true;
}

bool sl::CollisionCellMap::occupiedCellsRemove(sl::EntityId entityId)
{
	auto entityOpt = entityRegistry.getEntityById(entityId);

	if (!entityOpt.has_value()) { return false; }

	const sl::Entity& entity = entityOpt.value().get();

	sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
	if (!colisComp) { return false; }

	std::vector<sl::Cell> occupiedCells = colisComp->getOccupiedCells();

	for (auto& cell : occupiedCells) {
		auto it = cellToEntityIds.find(cell);
		if (it == cellToEntityIds.end()) { continue; }

		auto itEntity = std::find(it->second.begin(), it->second.end(), entity.getId());
		if (itEntity == it->second.end()) { continue; }

		sl::swapPopDelete(it->second, std::distance(it->second.begin(), itEntity));
	}

	colisComp->occupiedCellsClear();

	return true;
}


