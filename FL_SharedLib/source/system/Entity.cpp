#include "pch.h"
#include "system/Entity.hpp"
#include "network/Serializable.hpp"

namespace sl {
	Entity::~Entity() = default;

	Entity::Entity(sl::entityType type) {
		this->type = type;
	}
	Entity::Entity(std::string typeName) {
		this->type = sl::hashEntityType(typeName);
	}

	sl::Component* Entity::getComponent(TypeID ComponentTypeId) const {
		auto it = std::lower_bound(components.begin(), components.end(), ComponentTypeId,
			[](const auto& pair, const TypeID& tid) {
				return pair.first < tid;
			});
		if (it == components.end() || it->first != ComponentTypeId) return nullptr;
		return it->second.get();
	}

	bool Entity::hasComponent(TypeID ComponentTypeId) const {
		auto it = std::lower_bound(components.begin(), components.end(), ComponentTypeId,
			[](const auto& pair, const TypeID& tid) {
				return pair.first < tid;
			});
		if (it == components.end() || it->first != ComponentTypeId) return false;
		return true;
	}
}

