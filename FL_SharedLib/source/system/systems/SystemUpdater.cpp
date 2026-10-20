#include "pch.h"
#include "system/systems/SystemUpdater.hpp"
#include "system/WorldBase.hpp"
#include "system/WorldMap.hpp"
#include "system/Entity.hpp"
#include "system/systems/collision/CollisionSystem.hpp"
#include "system/systems/movement/MovementSystem.hpp"

sl::SystemUpdater::SystemUpdater(const SystemCreatorContext& context)
{
	creatorContext = std::make_unique<SystemCreatorContext>(context.collisionCellMap, context.entityRegistry);

	createSystems();
}

sl::SystemUpdater::~SystemUpdater() = default;

void sl::SystemUpdater::update(float deltaTime, const sl::WorldBase& world) {
	timeSinceLastUpdate += std::min(sf::seconds(deltaTime), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;
		float updateTimeCount = updateTime.asSeconds();

		onUpdate(updateTimeCount);

		for (auto& en : world.getEntities()) {

			onEnitiesUpdate(updateTimeCount, en);

		}
	}
}

void sl::SystemUpdater::onUpdate(float deltaTime) {
}

void sl::SystemUpdater::onEnitiesUpdate(float deltaTime, const sl::Entity& entity) {
	for (auto& system : systems) {
		system->update(deltaTime, entity);
	}
}

void sl::SystemUpdater::registerSystem(systemCreator creator) {
	getRegistry().push_back(std::move(creator));
}

void sl::SystemUpdater::createSystems() {
	if (!creatorContext) { return; }

	for (systemCreator& creator : getRegistry()) {
		systems.push_back(std::move(creator(*creatorContext)));
	}
}