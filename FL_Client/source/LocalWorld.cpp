#include "pch.hpp"
#include "LocalWorld.hpp"
#include "Entity.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "RenderManager.hpp"

LocalWorld::LocalWorld() : WorldBase() {}

LocalWorld::~LocalWorld() = default;

void LocalWorld::initializeRender(sf::RenderTarget& renderTarget)
{
	renderManager = std::make_unique<RenderManager>(renderTarget, playerEntity, entities, OnSetPlayerEntity);
}

void LocalWorld::setPlayerEntity(std::unique_ptr<sl::Entity>&& entity)
{
	if (!entity) { return; }

	playerEntity = std::move(entity);
	OnSetPlayerEntity.broadcast(playerEntity);
	
}

void LocalWorld::render()
{
	if (!renderManager) {
		std::cerr << "LocalWorld::render: renderManager is no valid" << "\n";
		return; 
	}
	renderManager->render();
}

void LocalWorld::onUpdate(float updateTime)
{
	if (!playerEntity) { return; }

	sl::MovementComponent* movComp = playerEntity->getComponent<sl::MovementComponent>();
	sl::TransformComponent* trComp = playerEntity->getComponent<sl::TransformComponent>();

	if (!movComp || !trComp) { return; }
	if (!movComp->isMoving()) { return; }

	trComp->setPosition(movComp->move(updateTime, trComp->getPosition()));
}
