#pragma once
#include "NetUtils.hpp"
#include <cstdint>
#include <string_view>
#include <nlohmann/json.hpp>
#include "EntityFactory.hpp"
#include <iostream>

using json = nlohmann::json;

namespace sl {
	class Entity;
}

#define COMPONENT_TYPE(name) \
    static constexpr std::string_view ComponentName = #name; \
    static constexpr uint32_t TypeId = sl::net::fnv1a(#name); \
    uint32_t getTypeId() const override { return TypeId; }

// registers the component in the factory
#define REGISTER_COMPONENT() \
	static void registerComponent(){ \
		try{ \
			sl::EntityFactory::registerComponent(ComponentName, std::function(initialize));\
		} \
		catch(std::exception& e){ \
			std::cerr << " [CRITICAL] component registration error " << ComponentName << ": " << e.what() << "\n"; \
			std::abort(); \
		} \
	} \
	inline static const bool isRegistered = (registerComponent(), true); \
	static void initialize(sl::ComponentInitContext context);

namespace sl {

	class Component {
	public:
		Component() = default;
		virtual ~Component() = default;
		virtual uint32_t getTypeId() const = 0;
	};
}