#include "pch.h"
#include "Entity.hpp"
#include "Serializable.hpp"

namespace sl {
	uint32_t Entity::nextId = 1;

	Entity::~Entity() = default;

	Entity::Entity() {
		this->id = nextId++;
	}
}