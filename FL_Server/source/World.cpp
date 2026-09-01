#include "pch.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "Serializer.hpp"
#include "ConnectionEvents.hpp"
#include "MovementSystem.hpp"
#include "WorldMap.hpp"
#include "CollisionSystem.hpp"
#include "NetworkIDComponent.hpp"
#include "ServerEntityFactory.hpp"
#include "ServerSystemUpdater.hpp"

World::World(ConnectionEvents& connectionEvents) : WorldBase(std::make_unique<SystemUpdaterClass>())
{
	entities.reserve(10000);
	entityFactory = std::make_unique<ServerEntityFactory>();

	connectionEvents.OnClientDisconnected.addFunction([this](uint32_t token) {
		removePlayerEntityByToken(token);
	});
}

World::~World() = default;


sl::EntityId World::addPlayerEntity(sl::EntityType entityType, const uint32_t& sessionToken, std::vector<uint8_t>& serializedEntityDataOut)
{
	sl::EntityId entityId = addEntity(entityType);

	tokenToEntityId.try_emplace(sessionToken, entityId);
	
	auto enPtr = entities.get(entityId);

	if (enPtr) {
		NetworkIDComponent* netComp = enPtr->getComponent<NetworkIDComponent>();

		if (netComp) {
			netComp->setSessionToken(sessionToken);
		}
		else {
			throw std::runtime_error("World::addPlayerEntity: PLayerEntity The entity does not have a NetworkIDComponent. AddPlayerEntity failed.");
		}

		enPtr->setId(entityId);
		worldMap->onEntityAdded(*enPtr);

		//temp
		auto* sysUpdater = dynamic_cast<ServerSystemUpdater*>(systemUpdater.get());
		if (!sysUpdater)
		{
			throw std::runtime_error("World::addPlayerEntity: dynamic_cast<ServerSystemUpdater*> failed. AddPlayerEntity failed.");
		}

		auto serializer = sysUpdater->getSerializer();

		if (!serializer) {
			throw std::runtime_error("World::addPlayerEntity: serializer is not valid. AddPlayerEntity failed.");
		}

		serializedEntityDataOut = serializer->serializeEntity(*enPtr);
		return entityId;
	}

	throw std::runtime_error("World::addPlayerEntity: PLayerEntity is missing on playerEntityStorage");

}

void World::removePlayerEntityByToken(uint32_t sessionToken)
{
	auto it = tokenToEntityId.find(sessionToken);
	if (it == tokenToEntityId.end()) { return; }

	entities.destroy(it->second);

	tokenToEntityId.erase(sessionToken);

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

sl::EntityId World::addEntity(sl::EntityType entityType)
{
	if (!entityFactory)
	{
		throw std::runtime_error("World::addEntity: entityFactory is not valid. addEntity failed.");
	}

	sl::Entity entity = entityFactory->createEntity(entityType);

	sl::EntityId entityId = entities.spawn(std::move(entity));

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

std::span<const sl::Entity> World::getEntities() const
{
	return entities.view();
}

std::span<sl::Entity> World::getEntities()
{
	return entities.view();
}

std::optional<std::reference_wrapper<const sl::Entity>> World::getPlayerEntityByToken(uint32_t token) const 
{
	auto it = tokenToEntityId.find(token);
	if (it == tokenToEntityId.end()) {
		return {};
	}
	return *entities.get(it->second);
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
