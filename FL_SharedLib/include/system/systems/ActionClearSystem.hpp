#pragma once
#include "system/systems/UpdatableSystem.hpp"

namespace sl {
	class ActionClearSystem : public sl::UpdatableSystem {
	public:
		ActionClearSystem() = default;
		void update(float updateTime, const sl::Entity& entity) override;
	private:
		RegisterUpdatableSystem();
	};
}