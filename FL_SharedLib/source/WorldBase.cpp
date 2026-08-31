#include "pch.h"
#include "WorldBase.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"
#include "MovementSystem.hpp"
#include "CollisionSystem.hpp"
#include "WorldMap.hpp"
#include "SystemUpdater.hpp"

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