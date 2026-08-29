#include "pch.h"
#include "EntityStorage.hpp"
#include "Entity.hpp"


void sl::EntityStorage::addEntity(std::unique_ptr<sl::Entity> entity, sl::EntityId id)
{
	if (!entity) { return; }

	if (idToIndex.find(id) == idToIndex.end())
	{
		entities.emplace_back(std::move(*entity));
		idToIndex.try_emplace(id, entities.size() - 1);
	}
}

void sl::EntityStorage::removeEntityById(sl::EntityId id)
{
	auto it = idToIndex.find(id);
	if (it == idToIndex.end()) { return ; }

	size_t removedIdx = it->second;
	size_t lastIdx = entities.size() - 1;

	if (removedIdx != lastIdx) {
		sl::EntityId movedId = entities[lastIdx].getId();

		entities[removedIdx] = std::move(entities[lastIdx]);

		idToIndex[movedId] = removedIdx;
	}

	idToIndex.erase(it);
	entities.pop_back();

}

sl::Entity* sl::EntityStorage::getEntityToId(sl::EntityId id)
{
	if (auto it = idToIndex.find(id); it != idToIndex.end()) {
		return &entities[it->second];
	}
	return {};
}

const sl::Entity* sl::EntityStorage::getEntityToId(sl::EntityId id) const {
	if (auto it = idToIndex.find(id); it != idToIndex.end()) {
		return &entities[it->second];
	}
	return {};
}

sl::Entity* sl::EntityStorage::getEntityToIndex(size_t index)
{
	if (index < entities.size()) {
		return &entities[index];
	}
	return {};
}

std::optional<size_t> sl::EntityStorage::getIndexById(sl::EntityId id) const
{
	auto it = idToIndex.find(id);
	if (it != idToIndex.end()) {
		return it->second;
	}
	return {};
}


