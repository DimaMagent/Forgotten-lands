#include "pch.h"
#include "system/systems/collision/CollisionCellMap.hpp"
#include "system/Entity.hpp"
#include "system/systems/collision/CollisionComponent.hpp"
#include "system/systems/TransformComponent.hpp"
#include "system/IEntityRegistry.hpp"
#include <cmath>
#include "system/Utils.hpp"


sl::CollisionCellMap::CollisionCellMap(const IEntityRegistry& entityRegistry):
	entityRegistry(entityRegistry)
{
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

	entityIdToDelegateTokens.try_emplace(entity.getId(), std::vector<std::shared_ptr<sl::DelegateToken>>());

	entityIdToDelegateTokens[entity.getId()].push_back(transComp->OnPositionChanged.addFunction([this, entityId](const sf::Vector2f& prevPos, const sf::Vector2f& newPos) {
		onPositionChanged(entityId, prevPos, newPos);
	}));

	entityIdToDelegateTokens[entity.getId()].push_back(colisComp->OnCollisionShapedChanged.addFunction([this, entityId](const sl::AABB& newAABB) {
		onCollisionShapeChanged(entityId, newAABB);
	}));

}

void sl::CollisionCellMap::removeEntityFromCollisionMap(const sl::Entity& entity)
{

	bool isSuccess = occupiedCellsRemove(entity.getId());

	if (!isSuccess) { return; }

	auto tokenIt = entityIdToDelegateTokens.find(entity.getId());
	if (tokenIt == entityIdToDelegateTokens.end()) { return; }

	entityIdToDelegateTokens.erase(tokenIt);
}

std::vector<sl::EntityId> sl::CollisionCellMap::getEntityIdsToCollisionMap(const sf::Vector2f& pos) const
{
	if (pos.x < 0 || pos.y < 0) { return  std::vector<sl::EntityId>(); }

	Cell cell(pos.x, pos.y);
	auto it = cellToEntityIds.find(cell);

	if (it == cellToEntityIds.end()) { return  std::vector<sl::EntityId>(); }

	return it->second;
}

bool sl::CollisionCellMap::getNearestEntityIdsToPosition(const sf::Vector2f& pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth) const
{
	Cell centerCell(pos.x, pos.y);

	cellIndex minX = centerCell.x - searchDepth;
	cellIndex maxX = centerCell.x + searchDepth;
	cellIndex minY = centerCell.y - searchDepth;
	cellIndex maxY = centerCell.y + searchDepth;

	for (cellIndex x = minX; x <= maxX; ++x) {
		for (cellIndex y = minY; y <= maxY; ++y) {

			Cell cell(x, y);

			auto it = cellToEntityIds.find(cell);
			if (it == cellToEntityIds.end()) { continue; }

			entityIdsOut.insert(entityIdsOut.end(), it->second.begin(), it->second.end());
		}
	}

	return true;
}

bool sl::CollisionCellMap::getNearestEntityIdsToEntity(const AABB& aabb, const sf::Vector2f& pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth) const
{
	if (!aabb.exists()) { return false; }

	Cell minCell(aabb.minX + pos.x, aabb.minY + pos.y);
	Cell maxCell(aabb.maxX + pos.x, aabb.maxY + pos.y);
		
	cellIndex minX = minCell.x - searchDepth;
	cellIndex maxX = maxCell.x + searchDepth;
	cellIndex minY = minCell.y - searchDepth;
	cellIndex maxY = maxCell.y + searchDepth;

	for (cellIndex x = minX; x <= maxX; ++x) {
		for (cellIndex y = minY; y <= maxY; ++y) {

			Cell cell(x, y);

			auto it = cellToEntityIds.find(cell);
			if (it == cellToEntityIds.end()) { continue; }

			entityIdsOut.insert(entityIdsOut.end(), it->second.begin(), it->second.end());
		}
	}

	return true;
}

uint8_t sl::CollisionCellMap::getSearchDepthByDistance(float distance)
{
	return static_cast<uint8_t>(ceil(distance / Cell::getCellSize()));
}

bool sl::CollisionCellMap::correctingEntityOnMap(sl::EntityId entityId, const sf::Vector2f& pos)
{
	bool isSuccess = occupiedCellsRemove(entityId);

	if (!isSuccess) { return false; }

	return occupiedCellsAdd(entityId, pos);
}

bool sl::CollisionCellMap::occupiedCellsAdd(sl::EntityId entityId, const sf::Vector2f& pos)
{
	auto entityOpt = entityRegistry.getEntityById(entityId);

	if (!entityOpt.has_value()) { return false; }

	const sl::Entity& entity = entityOpt.value().get();

	sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
	if (!colisComp) { return false; }

	AABB aabb = colisComp->getAABB();

	if (!aabb.exists()) { return false; }

	auto it = entityToCell.find(entityId);
	if (it == entityToCell.end()) {
		entityToCell.try_emplace(entityId, std::vector<Cell>());
	}

	cellIndex minCellX = static_cast<cellIndex>((aabb.minX + pos.x) / Cell::getCellSize());
	cellIndex maxCellX = static_cast<cellIndex>((aabb.maxX + pos.x) / Cell::getCellSize());
	cellIndex minCellY = static_cast<cellIndex>((aabb.minY + pos.y) / Cell::getCellSize());
	cellIndex maxCellY = static_cast<cellIndex>((aabb.maxY + pos.y) / Cell::getCellSize());

	for (cellIndex cellNumX = minCellX; cellNumX <= maxCellX; ++cellNumX) {
		for (cellIndex cellNumY = minCellY; cellNumY <= maxCellY; ++cellNumY) {
			Cell currentCell(cellNumX, cellNumY);
			auto it = cellToEntityIds.find(currentCell);

			if (it == cellToEntityIds.end()) {
				cellToEntityIds.try_emplace(currentCell, std::vector<sl::EntityId>());
			}
			cellToEntityIds[currentCell].push_back(entityId);

			entityToCell[entityId].push_back(currentCell);
		}
	}

	return true;
}

bool sl::CollisionCellMap::occupiedCellsRemove(sl::EntityId entityId)
{
	auto entityOpt = entityRegistry.getEntityById(entityId);

	if (!entityOpt.has_value()) { return false; }

	const sl::Entity& entity = entityOpt.value().get();

	auto cellsIt = entityToCell.find(entityId);
	if (cellsIt == entityToCell.end()) {
		return true;
	}

	std::vector<sl::Cell>& occupiedCells = cellsIt->second;


	for (auto& cell : occupiedCells) {
		auto it = cellToEntityIds.find(cell);
		if (it == cellToEntityIds.end()) { continue; }

		if (it->second.size() == 1 || it->second.size() == 0) {
			cellToEntityIds.erase(it);
			continue;
		}

		auto itEntity = std::find(it->second.begin(), it->second.end(), entity.getId());
		if (itEntity == it->second.end()) { continue; }

		sl::swapPopDelete(it->second, std::distance(it->second.begin(), itEntity));
	}

	entityToCell.erase(cellsIt);

	return true;
}

void sl::CollisionCellMap::onPositionChanged(sl::EntityId entityId, const sf::Vector2f& prevPos, const sf::Vector2f& newPos) {
	if (!Cell::isCellChanged(prevPos.x, prevPos.y, newPos.x, newPos.y)) { return; }

	if (!correctingEntityOnMap(entityId, newPos)) {
		std::cout << "sl::CollisionCellMap::recordEntityToCollisionMap: correctingEntityOnMap failed\n";
	}
}

void sl::CollisionCellMap::onCollisionShapeChanged(sl::EntityId entityId, const sl::AABB& newAABB) {

	auto entityOpt = entityRegistry.getEntityById(entityId);

	if (!entityOpt.has_value()) { return; }

	const sl::Entity& entity = entityOpt.value().get();

	sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
	if (!transComp) { return; }

	if (!correctingEntityOnMap(entityId, transComp->getPosition())) {
		std::cout << "sl::CollisionCellMap::recordEntityToCollisionMap: correctingEntityOnMap failed\n";
	}
}
