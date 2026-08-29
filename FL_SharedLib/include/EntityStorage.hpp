#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <span>
#include "EntityId.hpp"

namespace sl {
	class Entity;


	struct EntityStorage {

		const std::vector<sl::Entity>& getEntities() const { return entities; }

		const std::span<sl::Entity>& getEntities() { return entities; }

		//using ID as key for storage
		void addEntity(std::unique_ptr<sl::Entity> entity, sl::EntityId id);

		//Remove is performed using swap&pop.
		void removeEntityById(sl::EntityId id);

		sl::Entity* getEntityToId(sl::EntityId id);

		const sl::Entity* getEntityToId(sl::EntityId id) const;

		sl::Entity* getEntityToIndex(size_t index);

		std::optional<size_t> getIndexById(sl::EntityId id) const;
	protected:
		std::vector<sl::Entity> entities;
		std::unordered_map<sl::EntityId, size_t> idToIndex;
	};

}