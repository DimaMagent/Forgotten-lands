#include "pch.h"
#include "ComponentRegistry.hpp"
#include "Component.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"


std::unique_ptr<sl::Component> sl::ComponentRegistry::create(const std::string& name) const
{
    auto it = creators.find(name);
    if (it == creators.end()) return nullptr;
    return it->second();
}
