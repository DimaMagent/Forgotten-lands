#pragma once
#include "WorldBase.hpp"
#include "LockFreeDelegate.hpp"
#include <unordered_map>
#include <cstdint>
#include <optional>
#include "SlotMap.hpp"
#include "Entity.hpp"

class Serializer;
class ConnectionEvents;


class World : public sl::WorldBase {
public:
	sl::LockFreeDelegate<float> OnUpdate;

	World(ConnectionEvents& connectionEvents);

	virtual ~World();

	sl::EntityId addEntity(sl::EntityType entityType);

	sl::EntityId addPlayerEntity(sl::EntityType entityType, const uint32_t& sessionToken, std::vector<uint8_t>& serializedEntityDataOut);

	void removePlayerEntityByToken(uint32_t sessionToken);

	void removeEntityById(sl::EntityId id) override;

	std::optional<std::reference_wrapper<const sl::Entity>> getEntityById(sl::EntityId id) const override;

	std::optional<std::reference_wrapper<sl::Entity>> getEntityById(sl::EntityId id) override;

	const std::vector<sl::Entity>& getEntities() const override;

	auto getEntities();

	std::optional<std::reference_wrapper<sl::Entity>> getPlayerEntityByToken(uint32_t token);

	std::optional<uint32_t> getTokenById(sl::EntityId id) const;

protected:
	void onUpdate(float updateTime) override;

	void onUpdateEntities(sl::Entity& en, float updateTime) override;

private:
	sl::SlotMap<sl::Entity> entities;

	std::unique_ptr<Serializer> serializer;
	std::unique_ptr<ServerEntityFactory> entityFactory;

	std::unordered_map<uint32_t, sl::EntityId> tokenToEntityId;
};
