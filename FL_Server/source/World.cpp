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

	uint32_t entityId = entity->getGlobalId();

	NetworkIDComponent* netComp = entity->getComponent<NetworkIDComponent>();

	if (netComp) {
		netComp->setSessionToken(sessionToken);
	}
	else {
		throw ("World::addPlayerEntity: PLayerEntity The entity does not have a NetworkIDComponent. AddPlayerEntity failed.");
	}

	addEntity(std::move(entity), entity->getGlobalId());

	tokenToEntityId.try_emplace(sessionToken, entityId);
	
	auto enPtr = entities.getEntityToId(entityId).lock();

	if (enPtr) {
		return serializer->serializeEntity(*enPtr);
	}

	throw std::runtime_error("World::addPlayerEntity: PLayerEntity is missing on playerEntityStorage");

}

bool World::removePlayerEntityByToken(uint32_t sessionToken)
{
	auto it = tokenToEntityId.find(sessionToken);
	if (it == tokenToEntityId.end()) { return false; }

	bool isSucess = entities.removeEntityById(it->second);

	if (isSucess) {
		tokenToEntityId.erase(sessionToken);
	}

	return isSucess;
}

bool World::removeEntityById(uint32_t id)
{
	auto entity = getEntityById(id);
	if (!entity.has_value()) { return false; }

	NetworkIDComponent* netComp = entity.value().get().getComponent<NetworkIDComponent>();

	if (!netComp) { return false; }

	bool isSucess = entities.removeEntityById(id);

	if (isSucess)
	{
		tokenToEntityId.erase(netComp->getSessionToken());
	}

	return isSucess;
}

std::weak_ptr<sl::Entity> World::getPlayerEntityByToken(uint32_t token) const
{
	auto it = tokenToEntityId.find(token);
	if (it == tokenToEntityId.end()) {
		return std::weak_ptr<sl::Entity>();
	}
	return entities.getEntityToId(it->second);
}

std::optional<uint32_t> World::getTokenById(uint32_t id) const
{
	auto entity = getEntityById(id);
	if (!entity.has_value()) { return {}; }

	NetworkIDComponent* netComp = entity.value().get().getComponent<NetworkIDComponent>();
	
	if (!netComp) { return {}; }

	return netComp->getSessionToken();

}
