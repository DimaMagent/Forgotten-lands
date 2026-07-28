#pragma once
#include "WorldBase.hpp"
#include "LockFreeDelegate.hpp"
#include "EntityStorage.hpp"
#include <unordered_map>
#include <cstdint>

class Serializer;
class ConnectionEvents;


class World : public sl::WorldBase {
public:
	sl::LockFreeDelegate<float> OnUpdate;
	World(ConnectionEvents& connectionEvents);
	virtual ~World();
	//returns serialized playerEntity data
	std::vector<uint8_t> addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken);
	bool removePlayerEntityUsingToken(const uint32_t& sessionToken);
	bool removePlayerEntityUsingIndex(const size_t& index);
	std::weak_ptr<sl::Entity> getPlayerEntityToToken(uint32_t token) const;
protected:
	virtual void onUpdate(float updateTime) override;
	virtual void onUpdateEntities(sl::Entity& en, float updateTime) override;

private:
	std::unique_ptr<Serializer> serializer;

	//stores entities by tokens
	sl::EntityStorage playerEntityStorage;
};
