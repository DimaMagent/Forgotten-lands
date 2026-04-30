#pragma once
#include "WorldBase.hpp"
#include <memory>
#include <vector>
#include "Delegate.hpp"
#include "SFML/System/Time.hpp"

namespace sl {
	class Entity;
}
namespace sf {
	class RenderTarget;
}

class LocalWorld: public sl::WorldBase {
public:
	sl::Delegate<const std::weak_ptr<sl::Entity>> OnSetPlayerEntity;
	LocalWorld(sf::RenderTarget& renderTarget);
	~LocalWorld();
	void setPlayerEntity(std::unique_ptr<sl::Entity>&& entity);
	void render();
protected:
	std::shared_ptr<sl::Entity> playerEntity;
	sf::RenderTarget& renderTarget;

	virtual void onUpdate(float updateTime) override;
};