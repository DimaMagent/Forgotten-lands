#include "pch.h"
#include "system/systems/ActionClearSystem.hpp"
#include "system/systems/IntentComponent.hpp"
#include "system/Entity.hpp"

void sl::ActionClearSystem::update(float updateTime, const sl::Entity& entity) {
	sl::IntentComponent* intentComp = entity.getComponent<sl::IntentComponent>();

	if (!intentComp) { return; }

	if (!intentComp->isIntentChanged()) { return; }

	intentComp->clearActions();
}

sl::systemPtr sl::ActionClearSystem::initialize(sl::SystemCreatorContext& context) {
	return std::make_unique<sl::ActionClearSystem>();
}

sl::SystemProcessingPhase sl::ActionClearSystem::getProcessingPhase() {
	return sl::SystemProcessingPhase::PostSimulation;
}