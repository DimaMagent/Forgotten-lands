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
	serializer = std::make_unique<Serializer>();
	worldMap = std::make_unique<sl::WorldMap>();
	collisionSystem = std::make_unique<sl::CollisionSystem>(worldMap->getCollisionMap(), *this);
	connectionEvents.OnClientDisconnected.addFunction([this](uint32_t token) {
		removePlayerEntityUsingToken(token);
		});
}

World::~World() = default;


void World::onUpdate(float updateTime)
{
	for (auto& entity : playerEntityStorage.getEntities()) {
		if (!entity) { continue; }
		
		movementSystem->onUpdate(*entity, updateTime);
		collisionSystem->onUpdate(*entity, updateTime);
	}
	serializer->onUpdate(updateTime, playerEntityStorage);
	OnUpdate.broadcast(updateTime);
}

void World::onUpdateEntities(sl::Entity& en, float updateTime)
{
	collisionSystem->onUpdate(en, updateTime);
}

std::vector<uint8_t> World::addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken)
{
	if (!entity) {
		throw std::runtime_error("entity is nullptr, adding an entity is not possible");
	}

	playerEntityStorage.addEntity(std::move(entity), sessionToken);

	auto en = playerEntityStorage.getEntityToId(sessionToken).lock();

	if (en) {
		return serializer->serializeEntity(*en);
	}

	throw std::runtime_error("PLayerEntity is missing on playerEntityStorage");

}

bool World::removePlayerEntityUsingToken(const uint32_t& sessionToken)
{
	return playerEntityStorage.removeEntityUsingId(sessionToken);
}

bool World::removePlayerEntityUsingIndex(const size_t& index)
{
	return playerEntityStorage.removeEntityUsingIndex(index);
}

std::weak_ptr<sl::Entity> World::getPlayerEntityToToken(uint32_t token) const
{
	return playerEntityStorage.getEntityToId(token);
}

std::optional<std::reference_wrapper<sl::Entity>> World::getEntityById(uint32_t id) const
{
	auto entity = playerEntityStorage.getEntityToId(id).lock();
	if (entity) { return *entity; }

	return sl::WorldBase::getEntityById(id);
}
