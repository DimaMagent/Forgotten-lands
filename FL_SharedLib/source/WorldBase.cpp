#include "pch.h"
#include "WorldBase.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"
#include "MovementSystem.hpp"

sl::WorldBase::WorldBase()
{
	movementSystem = std::make_unique<MovementSystem>();
}

sl::WorldBase::~WorldBase() = default;

void sl::WorldBase::addEntity(std::unique_ptr<sl::Entity>&& entity, uint32_t id) {
	if (entity) {
		entities.addEntity(std::move(entity), id);
	}
}

void sl::WorldBase::update(float deltaTime) {
	timeSinceLastUpdate += std::min(sf::seconds(deltaTime), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;

		onUpdate(updateTime.asSeconds());

		for (auto& en : entities.getEntities()) {

			if (!en) { continue; }

			movementSystem->onUpdate(*en, deltaTime);
		}
	}
}

void sl::WorldBase::removeEntity(size_t index) {
	if (index >= entities.getEntities().size()) { return; }
	entities.removeEntityUsingIndex(index);
}