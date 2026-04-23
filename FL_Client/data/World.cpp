#include "pch.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "Character.hpp"

World::World(sf::RenderTarget& renderTarget) : renderTarget(renderTarget) {}

World::~World() = default;

void World::addEntity(std::unique_ptr<Entity> entity)
{
	if (entity) {
		Entities.push_back(std::move(entity));
	}
}

void World::setPlayerCharacter(std::unique_ptr<Character>&& character)
{
	playerCharacter = std::make_shared<Character>(std::move(*character));
	OnSetPlayerEntity.broadcast(playerCharacter);
}

void World::update(float deltaTime)
{

	timeSinceLastUpdate += std::min(sf::seconds(deltaTime), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;

		if (playerCharacter) {
			playerCharacter->move(updateTime.asSeconds());
		}
	}
	
}

void World::render()
{
	if (!Entities.empty()) {
		for (const auto& entity : Entities) {
			if (entity) {
				entity->render(renderTarget);
			}
		}
	}
	if (playerCharacter) {
		playerCharacter->render(renderTarget);
	}

}

/*Remove is performed using swap&pop.*/
void World::removeEntity(size_t index)
{
	if (index >= Entities.size()) { return; }
	Entities[index] = std::move(Entities.back());
	Entities.pop_back();
}
