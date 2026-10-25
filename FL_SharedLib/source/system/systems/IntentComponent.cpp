#include "pch.h"
#include "system/systems/IntentComponent.hpp"
#include "system/Entity.hpp"

bool sl::IntentComponent::hasAction(sl::Action action) const {
	return std::find(intentions.actions.begin(), intentions.actions.end(), action) != intentions.actions.end();
}

void sl::IntentComponent::initialize(sl::ComponentInitContext context) {
	context.entity.addComponent<sl::IntentComponent>();
}