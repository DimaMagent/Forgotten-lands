#include "pch.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Serializer.hpp"
#include "ConnectionEvents.hpp"
#include "MovementSystem.hpp"
#include "WorldMap.hpp"
#include "CollisionSystem.hpp"

World::World(ConnectionEvents& connectionEvents) : WorldBase()
{
	serializer = std::make_unique<Serializer>(*this);
	worldMap = std::make_unique<sl::WorldMap>();
	collisionSystem = std::make_unique<sl::CollisionSystem>(worldMap->getCollisionMap(), *this);
	connectionEvents.OnClientDisconnected.addFunction([this](uint32_t token) {
		removePlayerEntityByToken(token);
	});
}

World::~World() = default;

void World::onUpdate(float updateTime)
{
	collisionSystem->onUpdate(updateTime);
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

	size_t index = addEntity(std::move(entity), entity->getGlobalId());

	entityIdToToken.try_emplace(entityId, sessionToken);
	tokenToEntityId.try_emplace(sessionToken, entityId);
	
	auto enPtr = entities.getEntityToId(entityId).lock();

	if (enPtr) {
		worldMap->onEntityAdded(*enPtr);

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
		entityIdToToken.erase(it->second);
		tokenToEntityId.erase(sessionToken);
	}

	return isSucess;
}

bool World::removeEntityById(uint32_t id)
{
	auto it = entityIdToToken.find(id);

	bool isSucess = entities.removeEntityById(id);

	if (it != entityIdToToken.end() && isSucess)
	{
		entityIdToToken.erase(id);
		tokenToEntityId.erase(it->second);
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
	auto it = entityIdToToken.find(id);
	if (it == entityIdToToken.end()) { return {}; }

	return it->second;
}
