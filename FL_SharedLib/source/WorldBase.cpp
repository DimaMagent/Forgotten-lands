#include "pch.h"
#include "WorldBase.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"

sl::WorldBase::~WorldBase() = default;

void sl::WorldBase::addEntity(std::unique_ptr<sl::Entity>&& entity) {
	if (entity) {
		entities.push_back(std::move(entity));
	}
}

void sl::WorldBase::update(float deltaTime) {
	timeSinceLastUpdate += std::min(sf::seconds(deltaTime), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;

		onUpdate(updateTime.asSeconds());

		for (auto& en : entities) {

			if (!en) { continue; }

			sl::MovementComponent* movComp = en->getComponent<sl::MovementComponent>();
			if (!movComp || !movComp->isMoving()) { continue; }

			sl::TransformComponent* trComp = en->getComponent<sl::TransformComponent>();
			if (!trComp) { continue; }

			trComp->setPosition(movComp->move(updateTime.asSeconds(), trComp->getPosition()));
		}
	}
}

/*Remove is performed using swap&pop.*/
void sl::WorldBase::removeEntity(size_t index) {
	if (index >= entities.size()) { return; }
	entities[index] = std::move(entities.back());
	entities.pop_back();
}