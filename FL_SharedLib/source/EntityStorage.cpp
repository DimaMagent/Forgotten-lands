#include "pch.h"
#include "EntityStorage.hpp"
#include "Entity.hpp"


void sl::EntityStorage::addEntity(std::unique_ptr<sl::Entity> entity, uint32_t id)
{
	if (!entity) { return; }

	if (idToIndex.find(id) == idToIndex.end())
	{
		entities.emplace_back(std::move(entity));
		idToIndex.try_emplace(id, entities.size() - 1);
	}
}

bool sl::EntityStorage::removeEntityById(uint32_t id)
{
	auto it = idToIndex.find(id);
	if (it == idToIndex.end()) { return false; }

	size_t removedIdx = it->second;
	size_t lastIdx = entities.size() - 1;

	if (removedIdx != lastIdx) {
		uint32_t movedId = entities[lastIdx]->getGlobalId();

		entities[removedIdx] = std::move(entities[lastIdx]);

		idToIndex[movedId] = removedIdx;
	}

	idToIndex.erase(it);
	entities.pop_back();

	return true;
}

std::weak_ptr<sl::Entity> sl::EntityStorage::getEntityToId(uint32_t id) const
{
	if (auto it = idToIndex.find(id); it != idToIndex.end()) {
		auto& entity = entities[it->second];
		return entity;
	}
	return {};
}

std::weak_ptr<sl::Entity> sl::EntityStorage::getEntityToIndex(size_t index) const
{
	if (index < entities.size()) {
		return entities[index];
	}
	return {};
}

std::optional<size_t> sl::EntityStorage::getIndexById(uint32_t id) const
{
	auto it = idToIndex.find(id);
	if (it != idToIndex.end()) {
		return it->second;
	}
	return {};
}


