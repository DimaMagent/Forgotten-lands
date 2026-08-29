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

void LocalWorld::addPlayerEntity(std::unique_ptr<sl::Entity> entity, sl::EntityId id)
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

void LocalWorld::addEntity(std::unique_ptr<sl::Entity> entity, sl::EntityId id)
{
	if (!entity) {
		throw std::runtime_error("sl::WorldBase::addEntity: entity is nullptr, adding player entity is not possible");
	}

	entity->setId(id);

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
		OnSetPlayerEntity.broadcast(getPlayerEntity());
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

const std::vector<sl::Entity>& LocalWorld::getEntities() const
{
	return entities.getEntities();
}

auto LocalWorld::getEntities()
{
	return entities.getEntities();
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

void LocalWorld::onAbsenceEntityOnStatusPacket(sl::EntityId id)
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

std::optional<std::reference_wrapper<sl::Entity>> LocalWorld::getPlayerEntity()
{
	if (!isPlayerEntityAssigned) { return {}; }

	return *entities.getEntityToId(playerEntityId);
}
