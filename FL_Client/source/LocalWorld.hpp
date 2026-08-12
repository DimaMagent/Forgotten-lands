#pragma once
#include "WorldBase.hpp"
#include <memory>
#include <vector>
#include "Delegate.hpp"
#include "SFML/System/Time.hpp"
#include "EntityType.hpp"

namespace sl {
	class Entity;
	namespace net {
		struct EntityData;
	}
}
class RenderManager;
class StateManager;
class ClientEntityFactory;
class AnimationSystem;

class LocalWorld: public sl::WorldBase {
public:
	sl::Delegate<const std::weak_ptr<sl::Entity>> OnSetPlayerEntity;
	LocalWorld(std::weak_ptr<ClientEntityFactory> entityFactory, sf::RenderTarget& renderTarget);
	~LocalWorld();
	void addPlayerEntity(std::unique_ptr<sl::Entity> entity, uint32_t id);
	size_t addEntity(std::unique_ptr<sl::Entity> entity, uint32_t id) override;
	void render();
	bool removeEntityById(uint32_t id) override;
	std::weak_ptr<StateManager> getStateManager() const { return stateManager; }
protected:
	std::shared_ptr<spdlog::logger> game_logger;

	uint32_t playerEntityId;
	bool isPlayerEntityAssigned = false;

	std::unique_ptr<RenderManager> renderManager;
	std::shared_ptr<StateManager> stateManager;
	std::weak_ptr<ClientEntityFactory> entityFactory;
	std::unique_ptr<AnimationSystem> animationSystem;

	virtual void onUpdate(float updateTime) override;
	virtual void onUpdateEntities(sl::Entity& en, float updateTime) override;

	void onAbsenceEntity(const sl::net::EntityData& enData);
	void onAbsenceEntityOnStatusPacket(uint32_t id);
	void onAuth(const sl::net::EntityData& enData);
	const std::weak_ptr<sl::Entity> getPlayerEntity() const;
};