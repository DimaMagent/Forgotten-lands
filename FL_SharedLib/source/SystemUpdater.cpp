#include "pch.h"
#include "SystemUpdater.hpp"
#include "WorldBase.hpp"
#include "WorldMap.hpp"
#include "Entity.hpp"
#include "CollisionSystem.hpp"
#include "MovementSystem.hpp"

sl::SystemUpdater::SystemUpdater()
{
	collisionSystem = std::make_unique<sl::CollisionSystem>();
	movementSystem = std::make_unique<sl::MovementSystem>();

}

sl::SystemUpdater::~SystemUpdater() = default;

void sl::SystemUpdater::update(float deltaTime, const sl::WorldBase& world) {
	timeSinceLastUpdate += std::min(sf::seconds(deltaTime), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;
		float updateTimeCount = updateTime.asSeconds();

		onUpdate(updateTimeCount, world);

		for (auto& en : world.getEntities()) {

			onEnitiesUpdate(updateTimeCount, world, en);

		}
	}
}

void sl::SystemUpdater::onUpdate(float deltaTime, const sl::WorldBase& world) {

	auto worldMap = world.getWorldMap();

	if (worldMap.has_value()) {

		auto ColisMap = worldMap.value().get().getCollisionMap();

		if (ColisMap.has_value()) {
			collisionSystem->onUpdate(deltaTime, ColisMap.value().get(), world);
		}
	}
}

void sl::SystemUpdater::onEnitiesUpdate(float deltaTime, const sl::WorldBase& world, const sl::Entity& entity) {
	auto worldMap = world.getWorldMap();

	if (worldMap.has_value()) {
		auto ColisMap = worldMap.value().get().getCollisionMap();

		if (ColisMap.has_value()) {
			movementSystem->onUpdate(deltaTime, entity, ColisMap.value().get(), world);
		}
	}
}