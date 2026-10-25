#pragma once
#include "system/systems/UpdatableSystem.hpp"

namespace sl {
	class MovementDirectionUpdater : public sl::UpdatableSystem {
	public:
		MovementDirectionUpdater();
		void update(float updateTime, const sl::Entity& entity) override;

	private:
		RegisterUpdatableSystem();
	};
}