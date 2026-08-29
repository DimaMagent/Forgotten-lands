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

class LocalWorld: public sl::WorldBase {
public:
	sl::LockFreeDelegate<const std::optional<std::reference_wrapper<sl::Entity>>> OnSetPlayerEntity;

	LocalWorld(std::weak_ptr<ClientEntityFactory> entityFactory, sf::RenderTarget& renderTarget);

	virtual ~LocalWorld();

	void addEntity(std::unique_ptr<sl::Entity> entity, sl::EntityId id);

	void removeEntityById(sl::EntityId id) override;

	std::optional<std::reference_wrapper<const sl::Entity>> getEntityById(sl::EntityId id) const override;

	std::optional<std::reference_wrapper<sl::Entity>> getEntityById(sl::EntityId id) override;

	const std::vector<sl::Entity>& getEntities() const override;

	auto getEntities();

	void addPlayerEntity(std::unique_ptr<sl::Entity> entity, sl::EntityId id);

	void render();

	std::weak_ptr<StateManager> getStateManager() const { return stateManager; }

	std::optional<std::reference_wrapper<sl::Entity>> getPlayerEntity();

protected:
	std::shared_ptr<spdlog::logger> game_logger;

	sl::EntityStorage entities;

	sl::EntityId playerEntityId;
	bool isPlayerEntityAssigned = false;

	std::unique_ptr<RenderManager> renderManager;
	std::shared_ptr<StateManager> stateManager;
	std::weak_ptr<ClientEntityFactory> entityFactory;
	std::unique_ptr<AnimationSystem> animationSystem;

	virtual void onUpdate(float updateTime) override;

	virtual void onUpdateEntities(sl::Entity& en, float updateTime) override;

	void onAbsenceEntity(const sl::net::EntityData& enData);

	void onAbsenceEntityOnStatusPacket(sl::EntityId id);

	void onAuth(const sl::net::EntityData& enData);

};