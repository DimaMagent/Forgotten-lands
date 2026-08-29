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

	//returns serialized playerEntity data
	std::vector<uint8_t> addPlayerEntity(std::unique_ptr<sl::Entity> entity, const uint32_t& sessionToken);

	void removePlayerEntityByToken(uint32_t sessionToken);

	void removeEntityById(sl::EntityId id) override;

	sl::EntityId addEntity(std::unique_ptr<sl::Entity> entity);

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

	std::unordered_map<uint32_t, sl::EntityId> tokenToEntityId;
};
