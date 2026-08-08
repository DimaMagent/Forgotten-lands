#include "pch.hpp"
#include "ServerEntityStorage.hpp"
#include "Entity.hpp"

ServerEntityStorage::ServerEntityStorage(): EntityStorage()
{
}

void ServerEntityStorage::addEntity(std::unique_ptr<sl::Entity> entity, uint32_t id)
{
	if (entity) {
		entityIdToId.try_emplace(entity->getGlobalId(), id);
	}

	sl::EntityStorage::addEntity(std::move(entity), id);
}

bool ServerEntityStorage::removeEntityUsingId(const uint32_t& id)
{
	auto it = idToIndex.find(id);
	if (it == idToIndex.end()) { return false; }

	entityIdToId.erase(it->second);

	return sl::EntityStorage::removeEntityUsingId(id);
}

bool ServerEntityStorage::removeEntityUsingIndex(const size_t& index)
{
	auto it = indexToId.find(index);
	if (it == indexToId.end()) { return false; }

	return removeEntityUsingId(it->second);
}

std::weak_ptr<sl::Entity> ServerEntityStorage::getEntityToEntityId(uint32_t entityId) const
{
	if (auto it = entityIdToId.find(entityId); it != entityIdToId.end()) {
		if (auto it2 = idToIndex.find(it->second); it2 != idToIndex.end()) {
			auto& entity = entities[it2->second];
			return entity;
		}
	}
	return {};
}

uint32_t ServerEntityStorage::getEntityIdToIndex(size_t index) const
{
	if (index < entities.size()){
		return entities[index]->getGlobalId();
	}
	return 0;
}
