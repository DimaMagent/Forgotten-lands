#include "pch.hpp"
#include "LocalWorld.hpp"
#include "Entity.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "StateComponent.hpp"
#include "RenderManager.hpp"
#include "StateManager.hpp"
#include "LockFreeDelegate.hpp"
#include "ClientEntityFactory.hpp"
#include "AnimationSystem.hpp"
#include "MovementSystem.hpp"
#include "EntityType.hpp"
#include "StatusPacket.hpp"

LocalWorld::LocalWorld(std::weak_ptr<ClientEntityFactory> entityFactory, sf::RenderTarget& renderTarget) : WorldBase() ,
	stateManager(std::make_shared<StateManager>(playerEntity, entities, OnSetPlayerEntity)),
	entityFactory(entityFactory), renderManager(std::make_unique<RenderManager>(renderTarget))
{
	game_logger = spdlog::get("game");

	animationSystem = std::make_unique<AnimationSystem>(playerEntity, entities, OnSetPlayerEntity);

	stateManager->OnAbsenceEntity.addFunction([this](const sl::net::EntityData& enData) {this->onAbsenceEntity(enData); });
	stateManager->OnEntityAbsenceOnStatusPacket.addFunction([this](size_t entityIndex) {this->onAbsenceEntityOnStatusPacket(entityIndex); });
	stateManager->OnAuth.addFunction([this](const sl::net::EntityData& enData) {this->onAuth(enData); });
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

	movementSystem->onUpdate(*playerEntity, updateTime);

	animationSystem->onUpdate(updateTime);
}

void LocalWorld::onUpdateEntities(sl::Entity& en, float updateTime)
{
}

void LocalWorld::onAbsenceEntity(const sl::net::EntityData& enData)
{
	auto ef = entityFactory.lock();
	if (!ef) { return; }
	std::unique_ptr<sl::Entity> en = ef->entityCollection(enData);
	addEntity(std::move(en), enData.entityId);
}

void LocalWorld::onAbsenceEntityOnStatusPacket(uint32_t id)
{
	entities.removeEntityUsingId(id);
}

void LocalWorld::onAuth(const sl::net::EntityData& enData)
{
	auto ef = entityFactory.lock();
	if (!ef) { return; }
	setPlayerEntity(ef->entityCollection(enData));
	if (playerEntity) {
		playerEntity->setGlobalId(enData.entityId);
	}
	else {
		game_logger->error("failed to create entity with id {}", enData.entityId);
	}
}
