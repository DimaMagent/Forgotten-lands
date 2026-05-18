#include "pch.hpp"
#include "LocalWorld.hpp"
#include "Entity.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "RenderManager.hpp"
#include "StateManager.hpp"
#include "LockFreeDelegate.hpp"
#include "ClientEntityFactory.hpp"

LocalWorld::LocalWorld(std::weak_ptr<ClientEntityFactory> entityFactory) : WorldBase() ,
	stateManager(std::make_shared<StateManager>(playerEntity, entities, OnSetPlayerEntity)),
	entityFactory(entityFactory)
{
	stateManager->OnAbsenceEntity.addFunction([this](uint32_t globalId) {this->onAbsenceEntity(globalId); });
}

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

void LocalWorld::onAbsenceEntity(uint32_t globalId)
{
	auto ef = entityFactory.lock();
	if (!ef) { return; }
	std::unique_ptr<sl::Entity> en = ef->createEntity(sl::EntityType::Player);
	en->setGlobalId(globalId);
	addEntity(std::move(en));
}
