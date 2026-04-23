#pragma once
#include <memory>
#include <vector>
#include "Delegate.hpp"
#include "SFML/System/Time.hpp"

class Entity;
class Character;
namespace sf {
	class RenderTarget;
}

class World {
public:
	sl::Delegate<const std::weak_ptr<Character>> OnSetPlayerEntity;
	World(sf::RenderTarget& renderTarget);
	~World();
	void addEntity(std::unique_ptr<Entity> entity);
	void setPlayerCharacter(std::unique_ptr<Character>&& character);
	void render();
	void update(float deltaTime);
	void removeEntity(size_t index);
private:
	std::vector<std::unique_ptr<Entity>> Entities;
	std::shared_ptr<Character> playerCharacter;
	sf::RenderTarget& renderTarget;
	const sf::Time updateTime = sf::seconds(1.f/60.f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
};