#pragma once
#include <string>
#include <cstdint>
#include "system/Utils.hpp"

namespace sl {
	using entityType = uint32_t;

	inline entityType hashEntityType(const char* typeName) { return sl::fnv1a(typeName); }
	inline entityType hashEntityType(const std::string& typeName) { return sl::fnv1a(typeName.c_str()); }
}