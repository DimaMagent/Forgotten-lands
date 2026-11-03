#include "pch.h"
#include "system/systems/VisionComponent.hpp"
#include "system/Entity.hpp"

sl::VisionComponent::VisionComponent(float visionRadius) {
	this->visionRadius = visionRadius;
}

void sl::VisionComponent::initialize(sl::ComponentInitContext context) {
	if (!context.js) {
		context.entity.addComponent<sl::VisionComponent>();
		return;
	}
	context.entity.addComponent<sl::VisionComponent>(context.js->value("vision radius", 0.0f));
}