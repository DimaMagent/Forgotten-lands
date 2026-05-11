#pragma once
#include "WorldBase.hpp"
#include <unordered_map>
#include <cstdint>

class Serializer;

class World : public sl::WorldBase {
public:
	World();
	virtual ~World();
	void addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken);
	bool removePlayerEntityUsingToken(const uint32_t& sessionToken);
	bool removePlayerEntityUsingIndex(const size_t& index);
	std::weak_ptr<sl::Entity> getPlayerEntityToToken(uint32_t token) const;
protected:
	virtual void onUpdate(float updateTime) override;

private:
	std::unique_ptr<Serializer> serializer;
	std::vector<std::shared_ptr<sl::Entity>> playerEntities;
	std::unordered_map<uint32_t, size_t> tokenToIndex;
	std::unordered_map<size_t, uint32_t> indexToToken;
	int serializationFrequency = 20;
	int serializationCounter = 0;

};
