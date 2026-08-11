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

	size_t index = addEntity(std::move(entity), entity->getGlobalId());

	tokenToIndex.try_emplace(sessionToken, index);
	indexToToken.try_emplace(index, sessionToken);

	std::shared_ptr<sl::Entity> en = entities.getEntities()[index];

	if (en) {
		worldMap->onEntityAdded(*en);

		return serializer->serializeEntity(*en);
	}

	throw std::runtime_error("PLayerEntity is missing on playerEntityStorage");

}

bool World::removePlayerEntityByToken(uint32_t sessionToken)
{
	auto it = tokenToIndex.find(sessionToken);
	if (it == tokenToIndex.end()) { return false; }

	return removeEntityByIndex(it->second);
}

bool World::removeEntityByIndex(size_t index)
{
	if (index >= entities.getEntities().size()) { return false; }

	auto it = indexToToken.find(index);
	if (it == indexToToken.end()) { return false; }

	uint32_t token = it->second;

	if (index == entities.getEntities().size() - 1) {
		tokenToIndex.erase(token);
		indexToToken.erase(index);
		return entities.removeEntityByIndex(index);
	}

	auto movedIt = indexToToken.find(entities.getEntities().size() - 1);
	if (movedIt == indexToToken.end()) { return false; }

	tokenToIndex.erase(token);
	tokenToIndex[movedIt->second] = index;
	indexToToken[index] = movedIt->second;

	return entities.removeEntityByIndex(index);
}

bool World::removeEntityById(uint32_t id)
{
	auto indexOpt = entities.getIndexToId(id);
	if (!indexOpt.has_value()) { return false; }

	return removeEntityByIndex(indexOpt.value());
}

std::weak_ptr<sl::Entity> World::getPlayerEntityByToken(uint32_t token) const
{
	auto it = tokenToIndex.find(token);
	if (it == tokenToIndex.end()) {
		return std::weak_ptr<sl::Entity>();
	}
	return entities.getEntities()[it->second];
}

std::optional<uint32_t> World::getTokenByIndex(size_t index) const
{
	auto it = indexToToken.find(index);
	if (it == indexToToken.end()) { return {}; }

	return it->second;
}
