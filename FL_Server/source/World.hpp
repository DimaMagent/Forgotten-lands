#pragma once
#include "WorldBase.hpp"
#include "LockFreeDelegate.hpp"
#include "EntityStorage.hpp"
#include <unordered_map>
#include <cstdint>
#include <optional>

class Serializer;
class ConnectionEvents;
namespace sl {
	class Entity;
	class WorldMap;
	class CollisionSystem;
}


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
	virtual std::optional<std::reference_wrapper<sl::Entity>> getEntityById(uint32_t id) const override;
protected:
	virtual void onUpdate(float updateTime) override;
	virtual void onUpdateEntities(sl::Entity& en, float updateTime) override;

private:
	std::unique_ptr<Serializer> serializer;

	std::unique_ptr<sl::WorldMap> worldMap;

	std::unique_ptr<sl::CollisionSystem> collisionSystem;

	//stores entities by tokens
	sl::EntityStorage playerEntityStorage;
};
