#include "World.hpp"
#include "Entity.hpp"

World::World(sf::RenderTarget& renderTarget) : renderTarget(renderTarget) {}

World::~World() = default;

void World::addEntity(std::unique_ptr<Entity> entity)
{
	if (entity) {
		Entities.push_back(std::move(entity));
	}
}

void World::update(float deltaTime)
{

}

void World::render()
{
	if (Entities.empty()) { return; }
	for (const auto& entity : Entities) {
		if (entity) {
			entity->render(renderTarget);
		}
	}
}

/*Remove is performed using swap&pop.*/
void World::removeEntity(size_t index)
{
	if (index >= Entities.size()) { return; }
	Entities[index] = std::move(Entities.back());
	Entities.pop_back();
}
