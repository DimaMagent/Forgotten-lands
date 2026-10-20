#include "pch.hpp"
#include "ServerSystemUpdater.hpp"
#include "World.hpp"

ServerSystemUpdater::ServerSystemUpdater(const sl::SystemCreatorContext& context): SystemUpdater(context)
{
}

void ServerSystemUpdater::onUpdate(float deltaTime, const sl::WorldBase& world){
	SystemUpdater::onUpdate(deltaTime, world);
}

void ServerSystemUpdater::onEnitiesUpdate(float deltaTime, const sl::WorldBase& world, const sl::Entity& entity){
	SystemUpdater::onEnitiesUpdate(deltaTime, world, entity);
}