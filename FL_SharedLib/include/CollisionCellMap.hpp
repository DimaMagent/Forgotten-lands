#pragma once
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "Cell.hpp"
#include "Aabb.hpp"
#include "EntityId.hpp"

namespace sl {
	class Entity;
	class CollisionComponent;
	class WorldBase;

	class CollisionCellMap {
	public:
		
		CollisionCellMap(const WorldBase& world, cellIndex mapSizeX, cellIndex mapSizeY);
		
		void recordEntityToCollisionMap(const sl::Entity& entity);
		
		void removeEntityFromCollisionMap(const sl::Entity& entity);
		
		std::vector<sl::EntityId> getEntityIdsToCollisionMap(sf::Vector2f pos) const;
		
		[[nodiscard]] bool getNearestEntityIdsToPosition(sf::Vector2f pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth = 1) const;
		
		[[nodiscard]] bool getNearestEntityIdsToEntity(const AABB& aabb, sf::Vector2f pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth = 1) const;
		
		static uint8_t getSearchDepthByDistance(float distance);
	private:
		std::unordered_map<Cell, std::vector<sl::EntityId>> cellToEntityIds;

		std::unordered_map<sl::EntityId, uint64_t> entityIdToDelegateToken;

		const WorldBase& world;

		[[nodiscard]] bool onMapBound(const AABB& aabb, sf::Vector2f pos) const;
		[[nodiscard]] bool adjustingEntityOnMap(sl::EntityId entityId, sf::Vector2f pos);
		[[nodiscard]] bool occupiedCellsAdd(sl::EntityId entityId, sf::Vector2f pos);
		[[nodiscard]] bool occupiedCellsRemove(sl::EntityId entityId);
	};
}