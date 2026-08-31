#include "pch.hpp"
#include "ClientSystemUpdater.hpp"
#include <memory>
#include "AnimationSystem.hpp"


ClientSystemUpdater::ClientSystemUpdater() {
	animationSystem = std::make_unique<AnimationSystem>();
}

void ClientSystemUpdater::onUpdate(float deltaTime, const sl::WorldBase& world)
{
	SystemUpdater::onUpdate(deltaTime, world);

}

void ClientSystemUpdater::onEnitiesUpdate(float deltaTime, const sl::WorldBase& world, const sl::Entity& entity)
{
	SystemUpdater::onEnitiesUpdate(deltaTime, world, entity);

	animationSystem->onUpdate(deltaTime, entity);

}
