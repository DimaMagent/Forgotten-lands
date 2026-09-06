#include "pch.h"
#include "system/WorldBase.hpp"
#include "system/systems/movement/MovementComponent.hpp"
#include "system/systems/TransformComponent.hpp"
#include "system/Entity.hpp"
#include "system/systems/movement/MovementSystem.hpp"
#include "system/systems/collision/CollisionSystem.hpp"
#include "system/WorldMap.hpp"
#include "system/systems/SystemUpdater.hpp"

sl::WorldBase::WorldBase(std::unique_ptr<sl::SystemUpdater> currentSystemUpdater)
{
	systemUpdater = std::move(currentSystemUpdater);
	worldMap = std::make_unique<sl::WorldMap>(*this);	
}

sl::WorldBase::~WorldBase() = default;

void sl::WorldBase::update(float deltaTime) {
	systemUpdater->update(deltaTime, *this);
}

const std::optional<std::reference_wrapper<sl::WorldMap>> sl::WorldBase::getWorldMap() const {
	if (!worldMap) { return {}; }

	return *worldMap;
}