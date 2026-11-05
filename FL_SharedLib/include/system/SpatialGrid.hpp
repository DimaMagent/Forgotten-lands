#pragma once
#include <unordered_map>
#include <vector>
#include <cstdint>
#include "SFML/System/Vector2.hpp"
#include "system/Cell.hpp"
#include "system/EntityId.hpp"

namespace sl {
	class Entity;
	class DelegateToken;
	class IEntityRegistry;

	class SpatialGrid {
	public:
		SpatialGrid(const sl::IEntityRegistry& entityRegistry);

		void recordEntity(const sl::Entity& entity);

		void removeEntity(const sl::Entity& entity);

		std::vector<sl::EntityId> getEntityIds(const sf::Vector2f& pos) const;

		[[nodiscard]] bool getNearestEntityIdsToPosition(const sf::Vector2f& pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth = 1) const;

		[[nodiscard]] bool getNearestEntityIdsToEntity(const sf::Vector2f& entitySize, const sf::Vector2f& pos, std::vector<sl::EntityId>& entityIdsOut, uint8_t searchDepth = 1) const;

		static uint8_t getSearchDepthByDistance(float distance);
	private:
		std::unordered_map<Cell, std::vector<sl::EntityId>> cellToEntityIds;

		std::unordered_map<sl::EntityId, std::vector<Cell>> entityToCell;

		std::unordered_map<sl::EntityId, std::vector<std::shared_ptr<sl::DelegateToken>>> entityIdToDelegateTokens;

		const sl::IEntityRegistry& entityRegistry;

		[[nodiscard]] bool correctingEntityOnMap(sl::EntityId entityId, const sf::Vector2f& pos, const sf::Vector2f& entitySize);
		[[nodiscard]] bool occupiedCellsAdd(sl::EntityId entityId, const sf::Vector2f& pos, const sf::Vector2f& entitySize);
		[[nodiscard]] bool occupiedCellsRemove(sl::EntityId entityId);

		void onPositionChanged(sl::EntityId entityId, const sf::Vector2f& prevPos, const sf::Vector2f& newPos);

		void onSizeChanged(sl::EntityId entityId, const sf::Vector2f& newSize);
	};

}