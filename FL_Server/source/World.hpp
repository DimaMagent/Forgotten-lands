#pragma once
#include "WorldBase.hpp"
#include "LockFreeDelegate.hpp"
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
	std::vector<uint8_t> addPlayerEntity(std::unique_ptr<sl::Entity> entity, const uint32_t& sessionToken);
	bool removePlayerEntityByToken(uint32_t sessionToken);
	bool removeEntityByIndex(size_t index) override;
	bool removeEntityById(uint32_t id) override;

	std::weak_ptr<sl::Entity> getPlayerEntityByToken(uint32_t token) const;
	std::optional<uint32_t> getTokenByIndex(size_t index) const;

protected:
	virtual void onUpdate(float updateTime) override;
	virtual void onUpdateEntities(sl::Entity& en, float updateTime) override;

private:
	std::unique_ptr<Serializer> serializer;
	std::unique_ptr<sl::WorldMap> worldMap;
	std::unique_ptr<sl::CollisionSystem> collisionSystem;

	std::unordered_map<uint32_t, size_t> tokenToIndex;
	std::unordered_map<size_t, uint32_t> indexToToken;
};
