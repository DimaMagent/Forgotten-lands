#include "pch.h"
#include "system/systems/IntentComponent.hpp"
#include "system/Entity.hpp"

bool sl::IntentComponent::hasAction(sl::Action action) const {
	return intentions.isActionActive && intentions.action == action;
}

void sl::IntentComponent::initialize(sl::ComponentInitContext context) {
	context.entity.addComponent<sl::IntentComponent>();
}