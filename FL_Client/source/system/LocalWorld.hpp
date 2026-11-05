#pragma once
#include "system/WorldBase.hpp"
#include <memory>
#include <vector>
#include "system/handlers/LockFreeDelegate.hpp"
#include "SFML/System/Time.hpp"
#include "system/Entity.hpp"
#include "system/storages/SlotMap.hpp"
#include "network/DataProcessorTypes.hpp"
#include "system/IPlayerEntityStorage.hpp"
#include "system/INetworkEntityRegistry.hpp"

namespace sl {
	namespace net {
		struct EntityData;
	}
	class AttackSystem;
}
class RenderManager;
class StateManager;
class ClientEntityFactory;
class AnimationSystem;
class ClientSystemUpdater;

class LocalWorld: public sl::WorldBase, public IPlayerEntityStorage, public INetworkEntityRegistry {
public:
	sl::LockFreeDelegate<std::optional<sl::EntityId>> OnSetPlayerEntity;

	LocalWorld(sf::RenderTarget& renderTarget, DataProcessedDelegate& OnDataProcessed);

	virtual ~LocalWorld();

	sl::EntityId addEntity(sl::entityType entityType) override;

	sl::EntityId addEntity(const std::string& entityTypeName) override;

	// returns local entity id
	sl::EntityId addNetworkEntity(sl::entityType entityType, sl::EntityId networkId);

	// returns local entity id
	sl::EntityId addNetworkEntity(const std::string& entityType, sl::EntityId networkId);

	void setPlayerEntity(sl::EntityId id);

	void removeEntityById(sl::EntityId id) override;

	std::optional<sl::EntityId> getLocalIdByNetworkId(sl::EntityId networkId) const override;

	std::optional<std::reference_wrapper<const sl::Entity>> getEntityById(sl::EntityId id) const override;

	std::optional<std::reference_wrapper<sl::Entity>> getEntityById(sl::EntityId id) override;

	std::optional<std::reference_wrapper<const sl::Entity>> getEntityByNetworkId(sl::EntityId networkId) const override;

	std::optional<std::reference_wrapper<sl::Entity>> getEntityByNetworkId(sl::EntityId networkId) override;

	std::span<sl::Entity> getEntities() override;

	std::span<const sl::Entity> getEntities() const override;

	void render();

	std::weak_ptr<StateManager> getStateManager() const { return stateManager; }

	std::optional<std::reference_wrapper<sl::Entity>> getPlayerEntity() override;

	virtual std::optional<std::reference_wrapper<const sl::Entity>> getPlayerEntity() const override;

	std::optional<sl::EntityId> getPlayerEntityId() const override { return playerEntityId; }

protected:
	using SystemUpdaterClass = ClientSystemUpdater;

	std::shared_ptr<spdlog::logger> game_logger;

	sl::SlotMap<sl::Entity> entities;

	// stores network EntityId to local EntityId
	std::unordered_map<sl::EntityId, sl::EntityId> networkIdToLocalId;

	std::optional<sl::EntityId> playerEntityId;

	std::unique_ptr<RenderManager> renderManager;

	std::shared_ptr<StateManager> stateManager;

	std::unique_ptr<ClientEntityFactory> entityFactory;

	std::vector<std::shared_ptr<sl::DelegateToken>> tokens;

	void onAbsenceEntity(const sl::net::EntityData& enData);

	void onAbsenceEntityOnStatusPacket(sl::EntityId networkId);

	void onAuth(const sl::net::EntityData& enData);

	// returns local entity id
	sl::EntityId addNetworkEntity(sl::Entity&& entity, sl::EntityId networkId);

};