#include "pch.hpp"
#include "LocalWorld.hpp"
#include "system/Entity.hpp"
#include "game/render/RenderManager.hpp"
#include "network/StateManager.hpp"
#include "system/handlers/LockFreeDelegate.hpp"
#include "ClientEntityFactory.hpp"
#include "game/render/animations/AnimationSystem.hpp"
#include "system/EntityType.hpp"
#include "network/packets/StatusPacket.hpp"
#include "system/WorldMap.hpp"
#include "system/systems/ClientSystemUpdater.hpp"

LocalWorld::LocalWorld(sf::RenderTarget& renderTarget, DataProcessedDelegate& OnDataProcessed) : WorldBase(),
	stateManager(std::make_shared<StateManager>(*this, *this, OnDataProcessed)),
	entityFactory(std::make_unique<ClientEntityFactory>()), renderManager(std::make_unique<RenderManager>(renderTarget))
{
	game_logger = spdlog::get("game");

	tokens.push_back(stateManager->OnAbsenceEntity.addFunction([this](const sl::net::EntityData& enData) {this->onAbsenceEntity(enData); }));
	tokens.push_back(stateManager->OnEntityAbsenceOnStatusPacket.addFunction([this](sl::EntityId entityId) {this->onAbsenceEntityOnStatusPacket(entityId); }));
	tokens.push_back(stateManager->OnAuth.addFunction([this](const sl::net::EntityData& enData) {this->onAuth(enData); }));
}

LocalWorld::~LocalWorld() = default;

void LocalWorld::setPlayerEntity(sl::EntityId id) {
	playerEntityId = id;

	OnSetPlayerEntity.broadcast(playerEntityId);
}

sl::EntityId LocalWorld::addEntity(sl::entityType entityType)
{
	if (!entityFactory) {
		throw std::runtime_error("LocalWorld::addEntity entityFactory is not valid. addEntity failed.");
	}

	sl::Entity entity = entityFactory->createEntity(entityType);

	sl::EntityId entityId = entities.spawn(std::move(entity));

	entity.setId(entityId);

	auto enPtr = entities.get(entityId);

	if (enPtr) {
		worldMap->onEntityAdded(*enPtr);
	}
	return entityId;
}

sl::EntityId LocalWorld::addEntity(const std::string& entityType) {
	return addEntity(sl::hashEntityType(entityType));
}

sl::EntityId LocalWorld::addNetworkEntity(sl::entityType entityType, sl::EntityId networkId) {
	sl::EntityId entityId = addEntity(entityType);

	networkIdToLocalId.try_emplace(networkId, entityId);

	return entityId;
}

sl::EntityId LocalWorld::addNetworkEntity(const std::string& entityType, sl::EntityId networkId) {
	return addNetworkEntity(sl::hashEntityType(entityType), networkId);
}

void LocalWorld::render()
{
	if (!renderManager) {
		game_logger->error("LocalWorld::render: renderManager is no valid");
		return; 
	}
	if (!entities.view().empty()) {
		for (const auto& entity : entities.view()) {
			renderManager->render(entity);
		}
	}
}

void LocalWorld::removeEntityById(sl::EntityId id)
{
	if (playerEntityId.has_value() && id == playerEntityId.value()) {
		OnSetPlayerEntity.broadcast({});
		#ifdef DEBUG
		if (game_logger) {
			game_logger->info("PlayerEntity is unseted");
			}
		#endif // DEBUG

	}

	entities.destroy(id);
}

std::optional<std::reference_wrapper<const sl::Entity>> LocalWorld::getEntityById(sl::EntityId id) const
{
	auto entityPtr = entities.get(id);
	if (!entityPtr) { return {}; }

	return *entityPtr;
}

std::optional<std::reference_wrapper<sl::Entity>> LocalWorld::getEntityById(sl::EntityId id)
{
	auto entityPtr = entities.get(id);
	if (!entityPtr) { return {}; }

	return *entityPtr;
}

std::optional<std::reference_wrapper<const sl::Entity>> LocalWorld::getEntityByNetworkId(sl::EntityId networkId) const{
	return getEntityByNetworkId(networkId);
}

std::optional<std::reference_wrapper<sl::Entity>> LocalWorld::getEntityByNetworkId(sl::EntityId networkId) {
	auto id = getLocalIdByNetworkId(networkId);

	if (!id.has_value()) {
		return {};
	}

	auto entityPtr = entities.get(id.value());
	if (!entityPtr) { return {}; }

	return *entityPtr;
}

std::span<const sl::Entity> LocalWorld::getEntities() const
{
	return entities.view();
}

std::span<sl::Entity> LocalWorld::getEntities()
{
	return entities.view();
}

void LocalWorld::onAbsenceEntity(const sl::net::EntityData& enData)
{
	try {
		if (!entityFactory)
		{
			game_logger->error("LocalWorld::onAbsenceEntity: entityFactory is no valid");
			return;
		}

		addNetworkEntity(std::move(entityFactory->entityCollection(enData)), enData.entityId);
	}
	catch(std::exception& e){
		game_logger->error("LocalWorld::onAbsenceEntity exception: {}", e.what());
	}
}

void LocalWorld::onAbsenceEntityOnStatusPacket(sl::EntityId networkId)
{
	auto localEntityId =  getLocalIdByNetworkId(networkId);

	if (!localEntityId.has_value()) {
		game_logger->error("LocalWorld::onAbsenceEntityOnStatusPacket: no match found for the local entity id to the network entity id: {}", networkId.ID);
		return;
	}

	removeEntityById(localEntityId.value());
}

void LocalWorld::onAuth(const sl::net::EntityData& enData)
{
	try {
		if (!entityFactory)
		{ 
			game_logger->error("LocalWorld::onAuth: entityFactory is no valid");
			return;
		}

		sl::EntityId localEntityId = addNetworkEntity(std::move(entityFactory->entityCollection(enData)), enData.entityId);

		setPlayerEntity(localEntityId);

	}
	catch (std::exception& e) {
		game_logger->error("LocalWorld::onAuth: {}.Entity network id {}", e.what(), enData.entityId);
	}
}

std::optional<std::reference_wrapper<sl::Entity>> LocalWorld::getPlayerEntity()
{
	if (!playerEntityId.has_value()) { return {}; }

	return *entities.get(playerEntityId.value());
}

std::optional<std::reference_wrapper<const sl::Entity>> LocalWorld::getPlayerEntity() const
{
	if (!playerEntityId.has_value()) { return {}; }

	return *entities.get(playerEntityId.value());
}

std::optional<sl::EntityId> LocalWorld::getLocalIdByNetworkId(sl::EntityId networkId) const {
	auto it = networkIdToLocalId.find(networkId);

	if (it != networkIdToLocalId.end()) {
		return it->second;
	}

	return {};
}

sl::EntityId LocalWorld::addNetworkEntity(sl::Entity&& entity, sl::EntityId networkId)
{
	sl::EntityId entityId = entities.spawn(std::move(entity));

	auto enPtr = entities.get(entityId);

	networkIdToLocalId.try_emplace(networkId, entityId);

	if (enPtr) {
		enPtr->setId(entityId);

		worldMap->onEntityAdded(*enPtr);
	}

	return entityId;
}


