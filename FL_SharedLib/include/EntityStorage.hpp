#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>

namespace sl {
	class Entity;


	struct EntityStorage {

		const std::vector<std::shared_ptr<sl::Entity>>& getEntities() const { return entities; }

		//using ID as key for storage
		virtual void addEntity(std::unique_ptr<sl::Entity> entity, uint32_t id);

		//Remove is performed using swap&pop.
		virtual bool removeEntityById(uint32_t id);

		std::weak_ptr<sl::Entity> getEntityToId(uint32_t id) const;

		std::weak_ptr<sl::Entity> getEntityToIndex(size_t index) const;

		std::optional<size_t> getIndexById(uint32_t id) const;
	protected:
		std::vector<std::shared_ptr<sl::Entity>> entities;
		std::unordered_map<uint32_t, size_t> idToIndex;
	};

}