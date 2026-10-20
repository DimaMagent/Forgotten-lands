#include "pch.hpp"
#include "ServerSystemUpdater.hpp"
#include "World.hpp"

ServerSystemUpdater::ServerSystemUpdater(const sl::SystemCreatorContext& context): SystemUpdater(context)
{
}

void ServerSystemUpdater::onUpdate(float deltaTime){
	SystemUpdater::onUpdate(deltaTime);
}

void ServerSystemUpdater::onEnitiesUpdate(float deltaTime, const sl::Entity& entity){
	SystemUpdater::onEnitiesUpdate(deltaTime, entity);
}