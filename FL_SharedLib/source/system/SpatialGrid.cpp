#include "pch.h"
#include "system/SpatialGrid.hpp"
#include "system/Entity.hpp"
#include "system/systems/TransformComponent.hpp"
#include "system/IEntityRegistry.hpp"
#include "system/Utils.hpp"

sl::SpatialGrid::SpatialGrid(const sl::IEntityRegistry& entityRegistry):
	entityRegistry(entityRegistry){
}

void sl::SpatialGrid::recordEntity(const sl::Entity& entity) {

	sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
	if (!transComp) { return; }

	bool isSuccess = occupiedCellsAdd(entity.getId(), transComp->getPosition(), transComp->getSize());

	if (!isSuccess) { return; }

	sl::EntityId entityId = entity.getId();

	entityIdToDelegateTokens.try_emplace(entity.getId(), std::vector<std::shared_ptr<sl::DelegateToken>>());

	entityIdToDelegateTokens[entity.getId()].push_back(transComp->OnPositionChanged.addFunction([this, entityId](const sf::Vector2f& prevPos, const sf::Vector2f& newPos) {
		onPositionChanged(entityId, prevPos, newPos);
	}));

	entityIdToDelegateTokens[entity.getId()].push_back(transComp->OnSizeChanged.addFunction([this, entityId](const sf::Vector2f& newSize) {
		onSizeChanged(entityId, newSize);
	}));
}

void sl::SpatialGrid::removeEntity(const sl::Entity& entity) {
	bool isSuccess = occupiedCellsRemove(entity.getId());

	if (!isSuccess) { return; }

	auto tokenIt = entityIdToDelegateTokens.find(entity.getId());
	if (tokenIt == entityIdToDelegateTokens.end()) { return; }

	entityIdToDelegateTokens.erase(tokenIt);
}

std::vector<sl::EntityId> sl::SpatialGrid::getEntityIds(const sf::Vector2f& pos) const {
	return std::vector<sl::EntityId>();
	if (pos.x < 0 || pos.y < 0) { return  std::vector<sl::EntityId>(); }

	Cell cell(pos.x, pos.y);

	auto it = cellToEntityIds.find(cell);

	if (it == cellToEntityIds.end()) { return  std::vector<sl::EntityId>(); }

	return it->second;
}

bool sl::SpatialGrid::getNearestEntityIdsToPosition(const sf::Vector2f& pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth) const {
	Cell centerCell(pos.x, pos.y);

	cellIndex minX = centerCell.x - searchDepth;
	cellIndex maxX = centerCell.x + searchDepth;
	cellIndex minY = centerCell.y - searchDepth;
	cellIndex maxY = centerCell.y + searchDepth;

	for (cellIndex x = minX; x <= maxX; ++x) {
		for (cellIndex y = minY; y <= maxY; ++y) {
			if (x < 0 || y < 0) { continue; }

			Cell cell(x, y);

			auto it = cellToEntityIds.find(cell);
			if (it == cellToEntityIds.end()) { continue; }

			entityIdsOut.insert(entityIdsOut.end(), it->second.begin(), it->second.end());
			
		}
	}

	return true;
}

bool sl::SpatialGrid::getNearestEntityIdsToEntity(const sf::Vector2f& entitySize, const sf::Vector2f& pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth) const {

	Cell minCell(pos.x, pos.y);
	Cell maxCell(entitySize.x + pos.x, entitySize.y + pos.y);

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

uint8_t sl::SpatialGrid::getSearchDepthByDistance(float distance) {
	return static_cast<uint8_t>(ceil(distance / Cell::getCellSize()));
}

bool sl::SpatialGrid::correctingEntityOnMap(sl::EntityId entityId, const sf::Vector2f& pos, const sf::Vector2f& entitySize) {
	bool isSuccess = occupiedCellsRemove(entityId);

	if (!isSuccess) { return false; }

	return occupiedCellsAdd(entityId, pos, entitySize);
}

bool sl::SpatialGrid::occupiedCellsAdd(sl::EntityId entityId, const sf::Vector2f& pos, const sf::Vector2f& entitySize) {
	auto entityOpt = entityRegistry.getEntityById(entityId);

	if (!entityOpt.has_value()) { return false; }

	const sl::Entity& entity = entityOpt.value().get();

	auto it = entityToCell.find(entityId);
	if (it == entityToCell.end()) {
		entityToCell.try_emplace(entityId, std::vector<Cell>());
	}

	cellIndex minCellX = static_cast<cellIndex>(pos.x / Cell::getCellSize());
	cellIndex maxCellX = static_cast<cellIndex>((entitySize.x + pos.x) / Cell::getCellSize());
	cellIndex minCellY = static_cast<cellIndex>(pos.y / Cell::getCellSize());
	cellIndex maxCellY = static_cast<cellIndex>((entitySize.y + pos.y) / Cell::getCellSize());

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

bool sl::SpatialGrid::occupiedCellsRemove(sl::EntityId entityId) {
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

void sl::SpatialGrid::onPositionChanged(sl::EntityId entityId, const sf::Vector2f& prevPos, const sf::Vector2f& newPos) {
	if (!Cell::isCellChanged(prevPos.x, prevPos.y, newPos.x, newPos.y)) { return; }

	auto entityOpt = entityRegistry.getEntityById(entityId);

	if (!entityOpt.has_value()) { return; }

	const sl::Entity& entity = entityOpt.value().get();

	sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
	if (!transComp) { return; }

	if (!correctingEntityOnMap(entityId, newPos, transComp->getSize())) {
		std::cout << "sl::SpatialGrid::recordEntity: correctingEntityOnMap failed\n";
	}
}

void sl::SpatialGrid::onSizeChanged(sl::EntityId entityId, const sf::Vector2f& newSize) {
	auto entityOpt = entityRegistry.getEntityById(entityId);

	if (!entityOpt.has_value()) { return; }

	const sl::Entity& entity = entityOpt.value().get();

	sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
	if (!transComp) { return; }

	if (!correctingEntityOnMap(entityId, transComp->getPosition(), newSize)) {
		std::cout << "sl::SpatialGrid::recordEntity: correctingEntityOnMap failed\n";
	}
}
