#include "pch.h"
#include "NetworkComponentRegistry.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "StateComponent.hpp"

NetworkComponentRegistry::NetworkComponentRegistry() {
	registerComponents();
}

sl::Serializable* NetworkComponentRegistry::createAndAttach(uint32_t typeId, sl::Entity& entity) const
{
	auto it = registry.find(typeId);
	if (it != registry.end()) {
		return it->second(entity);
	}
	return nullptr;
}

void NetworkComponentRegistry::registerComponents()
{
	registerComponent<sl::TransformComponent>();
	registerComponent<sl::MovementComponent>();
	registerComponent<sl::StateComponent>();
}
