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
	class WorldBase;

	class CollisionCellMap {
	public:
		CollisionCellMap(const WorldBase& world, cellIndex mapSizeX, cellIndex mapSizeY);
		void recordEntityToCollisionMap(const sl::Entity& entity);
		void removeEntityFromCollisionMap(const sl::Entity& entity);
		std::vector<uint32_t> getEntityIdsToCollisionMap(sf::Vector2f pos) const;
		[[nodiscard]] bool getNearestEntityIdsToPosition(sf::Vector2f pos, std::vector<uint32_t>& entityIdsOut, uint8_t searchDepth = 1) const;
		[[nodiscard]] bool getNearestEntityIdsToEntity(const AABB& aabb, sf::Vector2f pos, std::vector<uint32_t>& entityIdsOut, uint8_t searchDepth = 1) const;
	private:
		//stores <cell, EntityIds>
		std::unordered_map<Cell, std::vector<uint32_t>> cellToEntityIds;

		//stores <EntityIds, DelegateToken> for remove
		std::unordered_map<uint32_t, uint64_t> entityIdToDelegateToken;

		const WorldBase& world;

		[[nodiscard]] bool onMapBound(const AABB& aabb, sf::Vector2f pos) const;
		[[nodiscard]] bool adjustingEntityOnMap(uint32_t entityId, sf::Vector2f pos);
		[[nodiscard]] bool occupiedCellsAdd(uint32_t entityId, sf::Vector2f pos);
		[[nodiscard]] bool occupiedCellsRemove(uint32_t entityId);
	};
}