#include "pch.hpp"
#include "ServerSystemUpdater.hpp"
#include "Serializer.hpp"
#include "World.hpp"
#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"

ServerSystemUpdater::ServerSystemUpdater()
{
	serializer = std::make_unique<Serializer>();
}

void ServerSystemUpdater::onUpdate(float deltaTime, const sl::WorldBase& world){
	SystemUpdater::onUpdate(deltaTime, world);

	const World* serverWorld = dynamic_cast<const World*>(&world);

	if (serverWorld) {
		serializer->onUpdate(deltaTime, serverWorld->getEntities(), *serverWorld);
	}
}

void ServerSystemUpdater::onEnitiesUpdate(float deltaTime, const sl::WorldBase& world, const sl::Entity& entity){
	SystemUpdater::onEnitiesUpdate(deltaTime, world, entity);
}