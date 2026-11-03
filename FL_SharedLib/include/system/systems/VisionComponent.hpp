#pragma once
#include "system/systems/Component.hpp"

namespace sl {

	class VisionComponent : public sl::Component {
	public:
		VisionComponent(float visionRadius = 0.0f);

		COMPONENT_TYPE(VisionComponent);

	private:
		float visionRadius;

		REGISTER_COMPONENT();
	};

}