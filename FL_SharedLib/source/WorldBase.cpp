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

		for (auto& en : getEntities()) {

			if (ColisMap.has_value()) {
				movementSystem->onUpdate(updateTimeCount, en, ColisMap.value().get(), *this);
			}

			onUpdateEntities(en, updateTimeCount);
		}
	}
}
