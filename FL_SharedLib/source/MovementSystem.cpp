#include "pch.h"
#include "Entity.hpp"
#include "MovementSystem.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "StateComponent.hpp"

sl::MovementSystem::MovementSystem()
{
}
void sl::MovementSystem::onUpdate(sl::Entity& entity, float updateTime) {

	sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
	sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
	sl::StateComponent* stateComp = entity.getComponent<sl::StateComponent>();

	if (!movComp || !trComp || !stateComp) { return; }

	if (!movComp->isMoving()) {
		if (!movComp->isMovementAlreadyReseted) {
			stateComp->movementState = sl::MovementState::None;
			movComp->isMovementAlreadyReseted = true;
		}
		return;
	}

	trComp->setPosition(movComp->move(updateTime, trComp->getPosition()));
	trComp->setRotation(movComp->getVelocityDirection());
	stateComp->movementState = sl::MovementState::Walk;
	movComp->isMovementAlreadyReseted = false;
}