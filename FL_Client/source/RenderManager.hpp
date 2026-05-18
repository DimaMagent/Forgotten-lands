#pragma once
#include <memory>
#include <vector>
#include "Delegate.hpp"

namespace sf {
	class RenderTarget;
}
namespace sl {
	class Entity;
}

class RenderManager {
public:
	RenderManager(sf::RenderTarget& renderTarget, std::weak_ptr<sl::Entity> playerEntity,
		std::vector<std::unique_ptr<sl::Entity>>& entities, sl::Delegate<const std::weak_ptr<sl::Entity>>& OnSetPlayerEntity);
	void render() const;
private:
	sf::RenderTarget& renderTarget;
	std::weak_ptr<sl::Entity> playerEntity;
	std::vector<std::unique_ptr<sl::Entity>>& entities;
};