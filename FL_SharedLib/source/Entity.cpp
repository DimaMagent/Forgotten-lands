#include "pch.h"
#include "Entity.hpp"
#include "Serializable.hpp"

namespace sl {
	uint32_t Entity::nextId = 1;

	Entity::~Entity() = default;

	Entity::Entity(sl::EntityType type) {
		this->id = nextId++;
		this->type = type;
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

