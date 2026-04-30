#include "pch.h"
#include "WorldBase.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"

sl::WorldBase::~WorldBase() {

}

void sl::WorldBase::addEntity(std::unique_ptr<sl::Entity>&& entity) {
	if (entity) {
		Entities.push_back(std::move(entity));
	}
}

void sl::WorldBase::update(float deltaTime) {
	timeSinceLastUpdate += std::min(sf::seconds(deltaTime), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;
		
		onUpdate(updateTime.asSeconds());

		for (auto& en : Entities) {

			if (!en) { break; }

			sl::MovementComponent* movComp = en->getComponent<sl::MovementComponent>();
			sl::TransformComponent* trComp = en->getComponent<sl::TransformComponent>();

			if (!movComp || !trComp) { break; }

			trComp->setPosition(movComp->move(updateTime.asSeconds(), trComp->getPosition()));
		}
	}
}

/*Remove is performed using swap&pop.*/
void sl::WorldBase::removeEntity(size_t index) {
	if (index >= Entities.size()) { return; }
	Entities[index] = std::move(Entities.back());
	Entities.pop_back();
}