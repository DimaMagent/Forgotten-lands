#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "Cell.hpp"
#include "Aabb.hpp"

namespace sl {
	class Entity;
	class CollisionComponent;

	class CollisionCellMap {
	public:
		CollisionCellMap(cellIndex mapSizeX, cellIndex mapSizeY);
		void recordEntityToCollisionMap(const sl::Entity& entity);
		void removeEntityFromCollisionMap(const sl::Entity& entity);
		std::vector<uint32_t> getEntityIdsToCollisionMap(sf::Vector2f pos) const;
		bool getNearestEntityIdsToPosition(sf::Vector2f pos, std::vector<uint32_t>& entityIdsOut, uint8_t searchDepth = 1) const;
		bool getNearestEntityIdsToEntity(const AABB& aabb, sf::Vector2f pos, std::vector<uint32_t>& entityIdsOut, uint8_t searchDepth = 1) const;
	private:
		//stores <cell, EntityIds>
		std::unordered_map<Cell, std::vector<uint32_t>> cellToEntityIds;

		//stores <EntityIds, DelegateToken> for remove
		std::unordered_map<uint32_t, uint64_t> entityIdToDelegateToken;

		bool onMapBound(const AABB& aabb, sf::Vector2f pos) const;
		bool adjustingEntityOnMap(const sl::Entity& entity, sf::Vector2f pos);
		bool occupiedCellsAdd(uint32_t EntityId, sf::Vector2f pos, sl::CollisionComponent& colisComp);
		bool occupiedCellsRemove(const sl::Entity& entity);
	};
}