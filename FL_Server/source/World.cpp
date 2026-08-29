#include "pch.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "Serializer.hpp"
#include "ConnectionEvents.hpp"
#include "MovementSystem.hpp"
#include "WorldMap.hpp"
#include "CollisionSystem.hpp"
#include "NetworkIDComponent.hpp"

World::World(ConnectionEvents& connectionEvents) : WorldBase()
{
	entities = sl::SlotMap<sl::Entity>(10000);
	serializer = std::make_unique<Serializer>(*this);
	connectionEvents.OnClientDisconnected.addFunction([this](uint32_t token) {
		removePlayerEntityByToken(token);
	});
}

World::~World() = default;

void World::onUpdate(float updateTime)
{
	serializer->onUpdate(updateTime);
	OnUpdate.broadcast(updateTime);
}

void World::onUpdateEntities(sl::Entity& en, float updateTime)
{
}

std::vector<uint8_t> World::addPlayerEntity(std::unique_ptr<sl::Entity> entity, const uint32_t& sessionToken)
{
	if (!entity) {
		throw std::runtime_error("World::addPlayerEntity: entity is nullptr, adding an entity is not possible");
	}

	NetworkIDComponent* netComp = entity->getComponent<NetworkIDComponent>();

	if (netComp) {
		netComp->setSessionToken(sessionToken);
	}
	else {
		throw ("World::addPlayerEntity: PLayerEntity The entity does not have a NetworkIDComponent. AddPlayerEntity failed.");
	}

	sl::EntityId entityId = addEntity(std::move(entity));

	tokenToEntityId.try_emplace(sessionToken, entityId);
	
	auto enPtr = entities.get(entityId);

	if (enPtr) {
		worldMap->onEntityAdded(*enPtr);
		return serializer->serializeEntity(*enPtr);
	}

	throw std::runtime_error("World::addPlayerEntity: PLayerEntity is missing on playerEntityStorage");

}

void World::removePlayerEntityByToken(uint32_t sessionToken)
{
	auto it = tokenToEntityId.find(sessionToken);
	if (it == tokenToEntityId.end()) { return; }

	tokenToEntityId.erase(sessionToken);

	entities.destroy(it->second);

}

void World::removeEntityById(sl::EntityId id)
{
	auto entity = entities.get(id);

	if (!entity) { return; }

	NetworkIDComponent* netComp = entity->getComponent<NetworkIDComponent>();

	if (netComp)
	{
		tokenToEntityId.erase(netComp->getSessionToken());
	}

	entities.destroy(id);

}

sl::EntityId World::addEntity(std::unique_ptr<sl::Entity> entity)
{
	if (!entity) {
		throw std::runtime_error("sl::World::addEntity: entity is nullptr, adding player entity is not possible");
	}
	sl::EntityId entityId = entities.spawn(std::move(*entity));

	auto entityPtr = entities.get(entityId);

	if (!entityPtr) {
		throw std::runtime_error("sl::World::addEntity: entity is missed in entities");
	}

	entityPtr->setId(entityId);

	worldMap->onEntityAdded(*entityPtr);

	return entityId;
}

std::optional<std::reference_wrapper<const sl::Entity>> World::getEntityById(sl::EntityId id) const
{
	auto enPtr = entities.get(id);
	if (!enPtr) { return {}; }

	return *enPtr;
}

std::optional<std::reference_wrapper<sl::Entity>> World::getEntityById(sl::EntityId id)
{
	auto enPtr = entities.get(id);
	if (!enPtr) { return {}; }

	return *enPtr;
}

const std::vector<sl::Entity>& World::getEntities() const
{
	return entities.view();
}

auto World::getEntities()
{
	return entities.view();
}

std::optional<std::reference_wrapper<sl::Entity>> World::getPlayerEntityByToken(uint32_t token)
{
	auto it = tokenToEntityId.find(token);
	if (it == tokenToEntityId.end()) {
		return {};
	}
	return *entities.get(it->second);
}

std::optional<uint32_t> World::getTokenById(sl::EntityId id) const
{
	auto entity = entities.get(id);
	if (!entity) { return {}; }

	NetworkIDComponent* netComp = entity->getComponent<NetworkIDComponent>();
	
	if (!netComp) { return {}; }

	return netComp->getSessionToken();

}
