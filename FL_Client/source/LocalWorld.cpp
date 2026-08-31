#include "pch.hpp"
#include "LocalWorld.hpp"
#include "Entity.hpp"
#include "RenderManager.hpp"
#include "StateManager.hpp"
#include "LockFreeDelegate.hpp"
#include "ClientEntityFactory.hpp"
#include "AnimationSystem.hpp"
#include "MovementSystem.hpp"
#include "EntityType.hpp"
#include "StatusPacket.hpp"
#include "WorldMap.hpp"
#include "ClientSystemUpdater.hpp"

LocalWorld::LocalWorld(sf::RenderTarget& renderTarget) : WorldBase(std::make_unique<ClientSystemUpdater>()) ,
	stateManager(std::make_shared<StateManager>(entities)),
	entityFactory(std::make_unique<ClientEntityFactory>()), renderManager(std::make_unique<RenderManager>(renderTarget))
{
	game_logger = spdlog::get("game");

	stateManager->OnAbsenceEntity.addFunction([this](const sl::net::EntityData& enData) {this->onAbsenceEntity(enData); });
	stateManager->OnEntityAbsenceOnStatusPacket.addFunction([this](sl::EntityId entityId) {this->onAbsenceEntityOnStatusPacket(entityId); });
	stateManager->OnAuth.addFunction([this](const sl::net::EntityData& enData) {this->onAuth(enData); });
}

LocalWorld::~LocalWorld() = default;

void LocalWorld::addPlayerEntity(sl::EntityType entityType, sl::EntityId id)
{
	addEntity(entityType, id);

	playerEntityId = id;

	isPlayerEntityAssigned = true;

	OnSetPlayerEntity.broadcast(playerEntityId);
	
}

void LocalWorld::addEntity(sl::EntityType entityType, sl::EntityId id)
{
	if (!entityFactory) {
		throw std::runtime_error("LocalWorld::addEntity entityFactory is not valid. addEntity failed.");
	}

	sl::Entity entity = entityFactory->createEntity(entityType);

	entity.setId(id);

	entities.addEntity(std::move(entity), id);

	auto enPtr = entities.getEntityToId(id);

	if (enPtr) {
		worldMap->onEntityAdded(*enPtr);
	}
}

void LocalWorld::render()
{
	if (!renderManager) {
		game_logger->error("LocalWorld::render: renderManager is no valid");
		return; 
	}
	if (!entities.getEntities().empty()) {
		for (const auto& entity : entities.getEntities()) {
			renderManager->render(entity);
		}
	}
}

void LocalWorld::removeEntityById(sl::EntityId id)
{
	if (id == playerEntityId) {
		isPlayerEntityAssigned = false;
		OnSetPlayerEntity.broadcast({});
		#ifdef DEBUG
		if (game_logger) {
			game_logger->info("PlayerEntity is unseted");
			}
		#endif // DEBUG

	}

	entities.removeEntityById(id);
}


std::optional<std::reference_wrapper<const sl::Entity>> LocalWorld::getEntityById(sl::EntityId id) const
{
	auto entityPtr = entities.getEntityToId(id);
	if (!entityPtr) { return {}; }

	return *entityPtr;
}

std::optional<std::reference_wrapper<sl::Entity>> LocalWorld::getEntityById(sl::EntityId id)
{
	auto entityPtr = entities.getEntityToId(id);
	if (!entityPtr) { return {}; }

	return *entityPtr;
}

std::span<const sl::Entity> LocalWorld::getEntities() const
{
	return entities.getEntities();
}

std::span<sl::Entity> LocalWorld::getEntities()
{
	return entities.getEntities();
}

void LocalWorld::onAbsenceEntity(const sl::net::EntityData& enData)
{
	try {
		if (!entityFactory)
		{
			game_logger->error("LocalWorld::onAbsenceEntity: entityFactory is no valid");
			return;
		}

		addEntity(std::move(entityFactory->entityCollection(enData)), enData.entityId);
	}
	catch(std::exception& e){
		game_logger->error("LocalWorld::onAbsenceEntity exception: {}", e.what());
	}
}

void LocalWorld::onAbsenceEntityOnStatusPacket(sl::EntityId id)
{
	removeEntityById(id);
}

void LocalWorld::onAuth(const sl::net::EntityData& enData)
{
	try {
		if (!entityFactory)
		{ 
			game_logger->error("LocalWorld::onAuth: entityFactory is no valid");
			return;
		}

		addPlayerEntity(std::move(entityFactory->entityCollection(enData)), enData.entityId);

	}
	catch (std::exception& e) {
		game_logger->error("{}.Entity id {}", e.what(), enData.entityId);
	}
}

std::optional<std::reference_wrapper<sl::Entity>> LocalWorld::getPlayerEntity()
{
	if (!isPlayerEntityAssigned) { return {}; }

	return *entities.getEntityToId(playerEntityId);
}

void LocalWorld::addEntity(sl::Entity&& entity, sl::EntityId id)
{
	entity.setId(id);

	entities.addEntity(std::move(entity), id);

	auto enPtr = entities.getEntityToId(id);

	if (enPtr) {
		worldMap->onEntityAdded(*enPtr);
	}
}

void LocalWorld::addPlayerEntity(sl::Entity&& entity, sl::EntityId id)
{
	addEntity(std::move(entity), id);

	playerEntityId = id;

	isPlayerEntityAssigned = true;

	OnSetPlayerEntity.broadcast(playerEntityId);
}