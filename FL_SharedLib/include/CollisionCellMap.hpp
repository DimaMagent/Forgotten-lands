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
		CollisionCellMap() = default;
		void recordEntityToCollisionMap(const sl::Entity& entity);
		void removeEntityToCollisionMap(const sl::Entity& entity);
		std::vector<uint32_t> getEntityIdsToCollisionMap(sf::Vector2f pos) const;
		std::vector<uint32_t> getNearestEntityIdsToPosition(sf::Vector2f pos, uint8_t searchDepth = 1) const;
		std::vector<uint32_t> getNearestEntityIdsToEntity(AABB aabb, sf::Vector2f pos, uint8_t searchDepth = 1) const;
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