#pragma once
#include "NetUtils.hpp"
#include <cstdint>
#include <string_view>

//TODO: нужно разобраться, почему ComponentName со значением,например, RenderComponent не то же самое, что строка со знчаением RenderComponent даже если ComponentName string,
// а не string_view. Затем нужно изменить в EntityFactory получение имени компонента

#define COMPONENT_TYPE(name) \
    static constexpr std::string_view ComponentName = #name; \
    static constexpr uint32_t TypeId = sl::net::fnv1a(#name); \
    uint32_t getTypeId() const override { return TypeId; }


namespace sl {

	class Component {
	public:
		Component() = default;
		virtual ~Component() = default;
		virtual uint32_t getTypeId() const = 0;
	};
}