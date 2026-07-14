#include "pch.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Serializer.hpp"
#include "ConnectionEvents.hpp"

World::World(ConnectionEvents& connectionEvents) : WorldBase()
{
	serializer = std::make_unique<Serializer>();
	connectionEvents.OnClientDisconnected.addFunction([this](uint32_t token) {
		removePlayerEntityUsingToken(token);
		});
}

World::~World() = default;


//probably should be moved to a separate function, but for now it's fine.
void World::onUpdate(float updateTime)
{
	for (auto& entity : playerEntityStorage.getEntities()) {
		if (!entity) { continue; }

		sl::MovementComponent* movComp = entity->getComponent<sl::MovementComponent>();
		if (!movComp || !movComp->isMoving()) { continue; }

		sl::TransformComponent* trComp = entity->getComponent<sl::TransformComponent>();
		if (!trComp) { continue; }

		trComp->setPosition(movComp->move(updateTime, trComp->getPosition()));
		trComp->setRotation(movComp->getVelocityDirection());
	}
	serializer->onUpdate(updateTime, playerEntityStorage);
	OnUpdate.broadcast(updateTime);
}

void World::addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken)
{
	if (!entity) { return; }

	playerEntityStorage.addEntity(std::move(entity), sessionToken);
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
