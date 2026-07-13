#include "pch.h"
#include "EntityStorage.hpp"
#include "Entity.hpp"


void EntityStorage::addEntity(std::unique_ptr<sl::Entity>&& entity, uint32_t id)
{
	if (entity) {
		entities.emplace_back(std::move(entity));
		idToIndex.try_emplace(id, entities.size() - 1);
		indexToId.try_emplace(entities.size() - 1, id);
	}
}

bool EntityStorage::removeEntityUsingId(const uint32_t& id)
{
	auto it = idToIndex.find(id);
	if (it == idToIndex.end()) { return false; }

	size_t removedIdx = it->second;
	size_t lastIdx = entities.size() - 1;

	if (removedIdx != lastIdx) {
		entities[removedIdx] = std::move(entities[lastIdx]);

		uint32_t movedToken = indexToId[lastIdx];
		idToIndex[movedToken] = removedIdx;
		indexToId[removedIdx] = movedToken;
	}

	idToIndex.erase(id);
	indexToId.erase(lastIdx);
	entities.pop_back();

	return true;
}

bool EntityStorage::removeEntityUsingIndex(const size_t& index)
{
	auto it = indexToId.find(index);
	if (it == indexToId.end()) { return false; }

	return removeEntityUsingId(it->second);
}

std::weak_ptr<sl::Entity> EntityStorage::getEntityToId(uint32_t id) const
{
	if (auto it = idToIndex.find(id); it != idToIndex.end()) {
		auto& entity = entities[it->second];
		return entity;
	}
	return {};
}


