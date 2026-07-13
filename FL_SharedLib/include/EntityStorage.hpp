#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

namespace sl {
	class Entity;
}

struct EntityStorage {
	
	const std::vector<std::shared_ptr<sl::Entity>>& getEntities() const { return entities; }

	//using ID as key for storage
	void addEntity(std::unique_ptr<sl::Entity>&& entity, uint32_t id);

	/*Remove is performed using swap&pop.*/
	bool removeEntityUsingId(const uint32_t& id);

	/*Remove is performed using swap&pop.*/
	bool removeEntityUsingIndex(const size_t& index);

	std::weak_ptr<sl::Entity> getEntityToId(uint32_t id) const;

private:
	std::vector<std::shared_ptr<sl::Entity>> entities;
	std::unordered_map<uint32_t, size_t> idToIndex;
	std::unordered_map<size_t, uint32_t> indexToId;
};