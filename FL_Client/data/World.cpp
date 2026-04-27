#include "pch.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "RenderComponent.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"

World::World(sf::RenderTarget& renderTarget) : renderTarget(renderTarget) {}

World::~World() = default;

void World::addEntity(std::unique_ptr<sl::Entity>&& entity)
{
	if (entity) {
		Entities.push_back(std::move(entity));
	}
}

void World::setPlayerEntity(std::unique_ptr<sl::Entity>&& entity)
{
	playerEntity = std::make_shared<sl::Entity>(std::move(*entity));
	OnSetPlayerEntity.broadcast(playerEntity);
}

void World::update(float deltaTime)
{

	timeSinceLastUpdate += std::min(sf::seconds(deltaTime), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;

		if (!playerEntity) { break; }

		sl::MovementComponent* movComp = playerEntity->getComponent<sl::MovementComponent>();
		sl::TransformComponent* trComp = playerEntity->getComponent<sl::TransformComponent>();

		if (!movComp || !trComp) { break; }

		trComp->setPosition(movComp->move(updateTime.asSeconds(), trComp->getPosition()));
		
	}
	
}

void World::render()
{
	if (!Entities.empty()) {
		for (const auto& entity : Entities) {
			if (!entity) { continue; }

			RenderComponent* comp = entity->getComponent<RenderComponent>();
			sl::TransformComponent* trComp = entity->getComponent<sl::TransformComponent>();

			if (!comp || !trComp) { continue; }

			comp->render(renderTarget, trComp->getPosition());
			
		}
	}
	if (playerEntity) {
		RenderComponent* comp = playerEntity->getComponent<RenderComponent>();
		sl::TransformComponent* trComp = playerEntity->getComponent<sl::TransformComponent>();
		if (comp && trComp) {
			comp->render(renderTarget, trComp->getPosition());
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
