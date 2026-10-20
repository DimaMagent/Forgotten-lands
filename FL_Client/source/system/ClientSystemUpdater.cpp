#include "pch.hpp"
#include "ClientSystemUpdater.hpp"
#include <memory>
#include "game/render/animations/AnimationSystem.hpp"


ClientSystemUpdater::ClientSystemUpdater(const sl::SystemCreatorContext& context): SystemUpdater(context) {
}

void ClientSystemUpdater::onUpdate(float deltaTime)
{
	SystemUpdater::onUpdate(deltaTime);

}

void ClientSystemUpdater::onEnitiesUpdate(float deltaTime, const sl::Entity& entity)
{
	SystemUpdater::onEnitiesUpdate(deltaTime, entity);

}
