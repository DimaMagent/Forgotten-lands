#include "pch.hpp"
#include "LocalWorld.hpp"
#include "Entity.hpp"
#include "RenderComponent.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"

LocalWorld::LocalWorld(sf::RenderTarget& renderTarget) : WorldBase(), renderTarget(renderTarget) {}

LocalWorld::~LocalWorld() = default;

void LocalWorld::setPlayerEntity(std::unique_ptr<sl::Entity>&& entity)
{
	if (!entity) { return; }

	playerEntity = std::move(entity);
	OnSetPlayerEntity.broadcast(playerEntity);
	
}

void LocalWorld::render()
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

void LocalWorld::onUpdate(float updateTime)
{
	if (!playerEntity) { return; }

	sl::MovementComponent* movComp = playerEntity->getComponent<sl::MovementComponent>();
	sl::TransformComponent* trComp = playerEntity->getComponent<sl::TransformComponent>();

	if (!movComp || !trComp) { return; }

	trComp->setPosition(movComp->move(updateTime, trComp->getPosition()));
}
