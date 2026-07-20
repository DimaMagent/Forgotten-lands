#pragma once

namespace sl {
	class Entity;

	class MovementSystem {
	public:
		MovementSystem();

		void onUpdate(sl::Entity& entity, float updateTime);
	};
}