#include "pch.h"
#include "WorldBase.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"

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

			sl::MovementComponent* movComp = en->getComponent<sl::MovementComponent>();
			if (!movComp || !movComp->isMoving()) { continue; }

			sl::TransformComponent* trComp = en->getComponent<sl::TransformComponent>();
			if (!trComp) { continue; }

			trComp->setPosition(movComp->move(updateTime.asSeconds(), trComp->getPosition()));
		}
	}
}

void sl::WorldBase::removeEntity(size_t index) {
	if (index >= entities.getEntities().size()) { return; }
	entities.removeEntityUsingIndex(index);
}