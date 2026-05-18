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

class LocalWorld: public sl::WorldBase {
public:
	sl::Delegate<const std::weak_ptr<sl::Entity>> OnSetPlayerEntity;
	LocalWorld();
	~LocalWorld();
	void initializeRender(sf::RenderTarget& renderTarget);
	void setPlayerEntity(std::unique_ptr<sl::Entity>&& entity);
	void render();
protected:
	std::shared_ptr<sl::Entity> playerEntity;
	std::unique_ptr<RenderManager> renderManager;

	virtual void onUpdate(float updateTime) override;
};