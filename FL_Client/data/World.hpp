#pragma once
#include <memory>
#include <vector>

class Entity;
namespace sf {
	class RenderTarget;
}

class World {
public:
	World(sf::RenderTarget& renderTarget);
	~World();
	void addEntity(std::unique_ptr<Entity> entity);
	void render();
	void update(float deltaTime);
	void removeEntity(size_t index);
private:
	std::vector<std::unique_ptr<Entity>> Entities;
	sf::RenderTarget& renderTarget;
};