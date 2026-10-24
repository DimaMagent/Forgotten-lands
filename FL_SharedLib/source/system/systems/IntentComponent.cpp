#include "system/systems/IntentComponent.hpp"
#include "system/Entity.hpp"

void sl::IntentComponent::initialize(sl::ComponentInitContext context) {
	context.entity.addComponent<sl::IntentComponent>();
}