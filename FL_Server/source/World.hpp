#pragma once
#include "WorldBase.hpp"
#include "IEntityTokenRegistry.hpp"
#include "LockFreeDelegate.hpp"
#include <unordered_map>
#include <cstdint>
#include <optional>
#include "SlotMap.hpp"
#include "Entity.hpp"

class Serializer;
class ConnectionEvents;
class ServerEntityFactory;
class ServerSystemUpdater;

class World : public sl::WorldBase, public IEntityTokenRegistry {
public:

	World(ConnectionEvents& connectionEvents);

	virtual ~World();

	sl::EntityId addEntity(sl::EntityType entityType);

	sl::EntityId addPlayerEntity(sl::EntityType entityType, const uint32_t& sessionToken, std::vector<uint8_t>& serializedEntityDataOut);

	void removePlayerEntityByToken(uint32_t sessionToken);

	void removeEntityById(sl::EntityId id) override;

	std::optional<std::reference_wrapper<const sl::Entity>> getEntityById(sl::EntityId id) const override;

	std::optional<std::reference_wrapper<sl::Entity>> getEntityById(sl::EntityId id) override;

	std::span<sl::Entity> getEntities() override;

	std::span<const sl::Entity> getEntities() const override;

	std::optional<std::reference_wrapper<const sl::Entity>> getPlayerEntityByToken(uint32_t token) const override;

	std::optional<std::reference_wrapper<sl::Entity>> getPlayerEntityByToken(uint32_t token) override;

	std::optional<uint32_t> getTokenById(sl::EntityId id) const override;

private:
	using SystemUpdaterClass = ServerSystemUpdater;

	sl::SlotMap<sl::Entity> entities;

	std::unique_ptr<ServerEntityFactory> entityFactory;

	std::unordered_map<uint32_t, sl::EntityId> tokenToEntityId;
};
