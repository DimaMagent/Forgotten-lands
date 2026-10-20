#include "pch.h"
#include "system/WorldBase.hpp"
#include "system/systems/movement/MovementComponent.hpp"
#include "system/systems/TransformComponent.hpp"
#include "system/Entity.hpp"
#include "system/WorldMap.hpp"
#include "system/systems/SystemUpdater.hpp"

sl::WorldBase::WorldBase()
{
	worldMap = std::make_unique<sl::WorldMap>(*this);	
}

sl::WorldBase::~WorldBase() = default;

void sl::WorldBase::update(float deltaTime) {
	timeSinceLastUpdate += std::min(sf::seconds(deltaTime), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;
		float updateTimeCount = updateTime.asSeconds();
		onUpdate(deltaTime);
	}
}

const std::optional<std::reference_wrapper<sl::WorldMap>> sl::WorldBase::getWorldMap() const {
	if (!worldMap) { return {}; }

	return *worldMap;
}

void sl::WorldBase::onUpdate(float deltaTime) {
}