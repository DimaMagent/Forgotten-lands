#include "pch.hpp"
#include "LocalWorld.hpp"
#include "Entity.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "RenderManager.hpp"
#include "StateManager.hpp"
#include "LockFreeDelegate.hpp"
#include "ClientEntityFactory.hpp"

LocalWorld::LocalWorld(std::weak_ptr<ClientEntityFactory> entityFactory, sf::RenderTarget& renderTarget) : WorldBase() ,
	stateManager(std::make_shared<StateManager>(playerEntity, entities, OnSetPlayerEntity)),
	entityFactory(entityFactory), renderManager(std::make_unique<RenderManager>(renderTarget))
{
	stateManager->OnAbsenceEntity.addFunction([this](uint32_t globalId) {this->onAbsenceEntity(globalId); });
	stateManager->OnEntityAbsenceOnStatusPacket.addFunction([this](size_t entityIndex) {this->onAbsenceEntityOnStatusPacket(entityIndex); });
}

LocalWorld::~LocalWorld() = default;

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
	if (!entities.empty()) {
		for (const auto& entity : entities) {
			if (!entity) { continue; }
			renderManager->render(*entity);
		}
	}
	if (playerEntity) {
		renderManager->render(*playerEntity);
	}
}

void LocalWorld::onUpdate(float updateTime)
{
	if (!playerEntity) { return; }

	sl::MovementComponent* movComp = playerEntity->getComponent<sl::MovementComponent>();
	sl::TransformComponent* trComp = playerEntity->getComponent<sl::TransformComponent>();

	if (!movComp || !trComp) { return; }
	if (!movComp->isMoving()) { return; }

	trComp->setPosition(movComp->move(updateTime, trComp->getPosition()));
	trComp->setRotation(movComp->getVelocityDirection());
}

void LocalWorld::onAbsenceEntity(uint32_t globalId)
{
	auto ef = entityFactory.lock();
	if (!ef) { return; }
	std::unique_ptr<sl::Entity> en = ef->createEntity(sl::EntityType::Player);
	en->setGlobalId(globalId);
	addEntity(std::move(en));
}

void LocalWorld::onAbsenceEntityOnStatusPacket(size_t entityIndex)
{
	removeEntity(entityIndex);
	std::cout << "Entity removed due to absence in status packet" << "\n";
}
