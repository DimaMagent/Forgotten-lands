#include "pch.h"
#include "StateComponent.hpp"

sl::StateComponent::StateComponent()
{
	actionState = ActionState::None;
	movementState = MovementState::None;
	lifeState = LifeState::Alive;
}
