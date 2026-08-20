#include "pch.h"
#include "WorldBase.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"
#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"
#include "WorldMap.hpp"

sl::WorldBase::WorldBase()
{
	worldMap = std::make_unique<sl::WorldMap>(*this);

	collisionSystem = std::make_unique<sl::CollisionSystem>();
	movementSystem = std::make_unique<MovementSystem>();
	
}

sl::WorldBase::~WorldBase() = default;

void sl::WorldBase::update(float deltaTime) {
	timeSinceLastUpdate += std::min(sf::seconds(deltaTime), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;

		float updateTimeCount = updateTime.asSeconds();

		auto ColisMap = worldMap->getCollisionMap();

		if (ColisMap.has_value()) {
			collisionSystem->onUpdate(updateTimeCount, ColisMap.value().get(), *this);
		}
		onUpdate(updateTimeCount);

		for (auto& en : entities.getEntities()) {

			if (!en) { continue; }

			if (ColisMap.has_value()) {
				movementSystem->onUpdate(updateTimeCount, *en, ColisMap.value().get(), *this);
			}

			onUpdateEntities(*en, updateTimeCount);
		}
	}
}

bool sl::WorldBase::removeEntityById(uint32_t id)
{
	return entities.removeEntityById(id);
}

void sl::WorldBase::addEntity(std::unique_ptr<sl::Entity> entity, uint32_t id)
{
	if (!entity) {
		throw std::runtime_error("sl::WorldBase::addEntity: entity is nullptr, adding player entity is not possible");
	}

	entities.addEntity(std::move(entity), id);

	auto enPtr = entities.getEntityToId(id).lock();
	if (enPtr) {
		worldMap->onEntityAdded(*enPtr);
	}
}

std::optional<std::reference_wrapper<sl::Entity>> sl::WorldBase::getEntityById(uint32_t id) const
{
	auto entityPtr = entities.getEntityToId(id).lock();
	if (!entityPtr) { return {}; }

	return *entityPtr;
}

const std::optional<std::reference_wrapper<sl::WorldMap>> sl::WorldBase::getWorldMap() const {
	if (!worldMap) { return {}; }

	return *worldMap;
}

