#pragma once
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

class World {
public:
	sl::Delegate<const std::weak_ptr<sl::Entity>> OnSetPlayerEntity;
	World(sf::RenderTarget& renderTarget);
	~World();
	void addEntity(std::unique_ptr<sl::Entity>&& entity);
	void setPlayerEntity(std::unique_ptr<sl::Entity>&& entity);
	void render();
	void update(float deltaTime);
	void removeEntity(size_t index);
private:
	std::vector<std::unique_ptr<sl::Entity>> Entities;
	std::shared_ptr<sl::Entity> playerEntity;
	sf::RenderTarget& renderTarget;
	const sf::Time updateTime = sf::seconds(1.f/60.f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
};