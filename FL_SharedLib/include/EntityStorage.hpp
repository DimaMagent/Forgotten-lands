#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <span>
#include "Entity.hpp"

namespace sl {
	class Entity;


	struct EntityStorage {

		EntityStorage(size_t capacity = 100);

		void reserve(size_t new_capacity) { entities.reserve(new_capacity); }

		size_t capacity() const { return entities.capacity(); }
		size_t size() const { return entities.size(); }

		std::span<const sl::Entity> getEntities() const { return entities; }

		std::span<sl::Entity> getEntities() { return entities; }

		//using ID as key for storage
		void addEntity(sl::Entity&& entity, sl::EntityId id);

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