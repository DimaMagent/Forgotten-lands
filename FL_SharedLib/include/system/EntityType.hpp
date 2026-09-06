#pragma once
#include <string>
#include <cstdint>

namespace sl {
	enum class EntityType: uint32_t {
		Player,
	};

	std::string EntityTypeToString(EntityType type);
}