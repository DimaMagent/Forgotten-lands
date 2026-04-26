#pragma once
#include <json.hpp>

namespace sl {


	class Component {
	public:
		Component() = default;
		virtual ~Component() = default;
		virtual void loadFromJSON(const nlohmann::json& data) = 0;
	};
}