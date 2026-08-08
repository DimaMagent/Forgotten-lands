#pragma once
#include "EntityStorage.hpp"

namespace sl {
	class Entity;
}

struct ServerEntityStorage : public sl::EntityStorage {
public:

	ServerEntityStorage();

	virtual void addEntity(std::unique_ptr<sl::Entity> entity, uint32_t id) override;

	virtual bool removeEntityUsingId(const uint32_t& id) override;

	virtual bool removeEntityUsingIndex(const size_t& index) override;

	std::weak_ptr<sl::Entity> getEntityToEntityId(uint32_t entityId) const;

	uint32_t getEntityIdToIndex(size_t index) const;
protected:
	std::unordered_map<uint32_t, uint32_t> entityIdToId;
};