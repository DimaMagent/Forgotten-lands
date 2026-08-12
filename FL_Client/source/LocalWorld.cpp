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
	stateManager(std::make_shared<StateManager>(entities)),
	entityFactory(entityFactory), renderManager(std::make_unique<RenderManager>(renderTarget))
{
	game_logger = spdlog::get("game");

	animationSystem = std::make_unique<AnimationSystem>(entities);

	stateManager->OnAbsenceEntity.addFunction([this](const sl::net::EntityData& enData) {this->onAbsenceEntity(enData); });
	stateManager->OnEntityAbsenceOnStatusPacket.addFunction([this](size_t entityIndex) {this->onAbsenceEntityOnStatusPacket(entityIndex); });
	stateManager->OnAuth.addFunction([this](const sl::net::EntityData& enData) {this->onAuth(enData); });
}

LocalWorld::~LocalWorld() = default;

void LocalWorld::addPlayerEntity(std::unique_ptr<sl::Entity> entity, uint32_t id)
{
	if (!entity) {
		throw std::runtime_error("LocalWorld::addPlayerEntity: entity is nullptr, adding player entity is not possible");
		return; 
	}

	addEntity(std::move(entity), id);

	playerEntityId = id;

	isPlayerEntityAssigned = true;

	OnSetPlayerEntity.broadcast(getPlayerEntity());
	
}

size_t LocalWorld::addEntity(std::unique_ptr<sl::Entity> entity, uint32_t id)
{
	if (!entity)
	{
		throw std::runtime_error("LocalWorld::addEntity: entity is nullptr, adding player entity is not possible");
	}

	entity->setGlobalId(id);

	return WorldBase::addEntity(std::move(entity), id);
}

void LocalWorld::render()
{
	if (!renderManager) {
		game_logger->error("LocalWorld::render: renderManager is no valid");
		return; 
	}
	if (!entities.getEntities().empty()) {
		for (const auto& entity : entities.getEntities()) {
			if (!entity) { continue; }

			renderManager->render(*entity);
		}
	}
}

bool LocalWorld::removeEntityById(uint32_t id)
{
	if (id == playerEntityId) {
		isPlayerEntityAssigned = false;
		OnSetPlayerEntity.broadcast(getPlayerEntity());
	}

	return entities.removeEntityById(id);
}

void LocalWorld::onUpdate(float updateTime)
{
	animationSystem->onUpdate(updateTime);
}

void LocalWorld::onUpdateEntities(sl::Entity& en, float updateTime)
{
}

void LocalWorld::onAbsenceEntity(const sl::net::EntityData& enData)
{
	auto ef = entityFactory.lock();
	if (!ef) 
	{ 
		game_logger->error("LocalWorld::onAbsenceEntity: entityFactory is no valid");
		return;
	}

	std::unique_ptr<sl::Entity> en = ef->entityCollection(enData);
	addEntity(std::move(en), enData.entityId);
}

void LocalWorld::onAbsenceEntityOnStatusPacket(uint32_t id)
{
	removeEntityById(id);
}

void LocalWorld::onAuth(const sl::net::EntityData& enData)
{
	try {
		auto ef = entityFactory.lock();
		if (!ef) 
		{ 
			game_logger->error("LocalWorld::onAuth: entityFactory is no valid");
			return;
		}

		addPlayerEntity(ef->entityCollection(enData), enData.entityId);

	}
	catch (std::exception& e) {
		game_logger->error("{}.Entity id {}", e.what(), enData.entityId);
	}
}

const std::weak_ptr<sl::Entity> LocalWorld::getPlayerEntity() const
{
	if (!isPlayerEntityAssigned) { return {}; }

	return entities.getEntityToId(playerEntityId);
}
