#include "pch.h"
#include "system/systems/movement/MovementDirectionUpdater.hpp"
#include "system/systems/movement/MovementComponent.hpp"
#include "system/systems/IntentComponent.hpp"
#include "system/Entity.hpp"

sl::MovementDirectionUpdater::MovementDirectionUpdater(){

}
void sl::MovementDirectionUpdater::update(float updateTime, const sl::Entity& entity) {
	sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
	sl::IntentComponent* intentComp = entity.getComponent<sl::IntentComponent>();

	if (!movComp || !intentComp) { return; }

	if (movComp->getVelocityDirection() != intentComp->getMovementDirectionIntentions()) {
		movComp->setVelocityDirection(sl::inBounds(intentComp->getMovementDirectionIntentions(), sf::Vector2i(-1, -1), sf::Vector2i(1, 1)));
	}
}

sl::systemPtr sl::MovementDirectionUpdater::initialize(sl::SystemCreatorContext& context) {
	return std::make_unique<sl::MovementDirectionUpdater>();
}

sl::SystemProcessingPhase sl::MovementDirectionUpdater::getProcessingPhase() {
	return sl::SystemProcessingPhase::Input;
}