#pragma once
#include "WorldBase.hpp"
#include <memory>
#include <vector>
#include "LockFreeDelegate.hpp"
#include "SFML/System/Time.hpp"
#include "EntityType.hpp"
#include "EntityStorage.hpp"
#include "EntityId.hpp"

namespace sl {
	class Entity;
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

class LocalWorld: public sl::WorldBase {
public:
	sl::LockFreeDelegate<std::optional<sl::EntityId>> OnSetPlayerEntity;

	LocalWorld(sf::RenderTarget& renderTarget);

	virtual ~LocalWorld();

	void addEntity(sl::EntityType entityType, sl::EntityId id);

	void addPlayerEntity(sl::EntityType entityType, sl::EntityId id);

	void removeEntityById(sl::EntityId id) override;

	std::optional<std::reference_wrapper<const sl::Entity>> getEntityById(sl::EntityId id) const override;

	std::optional<std::reference_wrapper<sl::Entity>> getEntityById(sl::EntityId id) override;

	std::span<sl::Entity> getEntities() override;

	std::span<const sl::Entity> getEntities() const override;

	void render();

	std::weak_ptr<StateManager> getStateManager() const { return stateManager; }

	std::optional<std::reference_wrapper<sl::Entity>> getPlayerEntity();

protected:
	using SystemUpdaterClass = ClientSystemUpdater;

	std::shared_ptr<spdlog::logger> game_logger;

	sl::EntityStorage entities;

	sl::EntityId playerEntityId;
	bool isPlayerEntityAssigned = false;

	std::unique_ptr<RenderManager> renderManager;
	std::shared_ptr<StateManager> stateManager;
	std::unique_ptr<ClientEntityFactory> entityFactory;

	void onAbsenceEntity(const sl::net::EntityData& enData);

	void onAbsenceEntityOnStatusPacket(sl::EntityId id);

	void onAuth(const sl::net::EntityData& enData);

	void addEntity(sl::Entity&& entity, sl::EntityId id);

	void addPlayerEntity(sl::Entity&& entity, sl::EntityId id);

};