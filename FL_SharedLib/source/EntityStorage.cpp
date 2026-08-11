#include "pch.h"
#include "EntityStorage.hpp"
#include "Entity.hpp"


void sl::EntityStorage::addEntity(std::unique_ptr<sl::Entity> entity, uint32_t id)
{
	if (entity) {
		entities.emplace_back(std::move(entity));
		idToIndex.try_emplace(id, entities.size() - 1);
		indexToId.try_emplace(entities.size() - 1, id);
	}
}

bool sl::EntityStorage::removeEntityById(const uint32_t& id)
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

bool sl::EntityStorage::removeEntityByIndex(const size_t& index)
{
	auto it = indexToId.find(index);
	if (it == indexToId.end()) { return false; }

	return removeEntityById(it->second);
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

std::optional<uint32_t> sl::EntityStorage::getIdToIndex(size_t index) const
{
	auto it = indexToId.find(index);
	if (it != indexToId.end()) {
		return it->second;
	}
	return {};
}

std::optional<size_t> sl::EntityStorage::getIndexToId(uint32_t id) const
{
	auto it = idToIndex.find(id);
	if (it != idToIndex.end()) {
		return it->second;
	}
	return {};
}


