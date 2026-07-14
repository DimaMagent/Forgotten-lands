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
	game_logger = spdlog::get("game");

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
		game_logger->warn("LocalWorld::render: renderManager is no valid");
		return; 
	}
	if (!entities.getEntities().empty()) {
		for (const auto& entity : entities.getEntities()) {
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
	addEntity(std::move(en), globalId);
}

void LocalWorld::onAbsenceEntityOnStatusPacket(uint32_t id)
{
	entities.removeEntityUsingId(id);
}
