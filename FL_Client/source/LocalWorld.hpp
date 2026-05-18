#pragma once
#include "WorldBase.hpp"
#include <memory>
#include <vector>
#include "Delegate.hpp"
#include "SFML/System/Time.hpp"

namespace sl {
	class Entity;
}
class RenderManager;
class StateManager;
class ClientEntityFactory;

class LocalWorld: public sl::WorldBase {
public:
	sl::Delegate<const std::weak_ptr<sl::Entity>> OnSetPlayerEntity;
	LocalWorld(std::weak_ptr<ClientEntityFactory> entityFactory);
	~LocalWorld();
	void initializeRender(sf::RenderTarget& renderTarget);
	void setPlayerEntity(std::unique_ptr<sl::Entity>&& entity);
	void render();
	std::weak_ptr<StateManager> getStateManager() const { return stateManager; }
protected:
	std::shared_ptr<sl::Entity> playerEntity;
	std::unique_ptr<RenderManager> renderManager;
	std::shared_ptr<StateManager> stateManager;
	std::weak_ptr<ClientEntityFactory> entityFactory;

	virtual void onUpdate(float updateTime) override;
	void onAbsenceEntity(uint32_t globalId);
};