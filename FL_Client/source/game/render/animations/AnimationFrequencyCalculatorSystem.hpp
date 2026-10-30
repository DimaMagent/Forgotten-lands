#pragma once
#include "system/systems/UpdatableSystem.hpp"

class AnimationFrequencyCalculatorSystem: public sl::UpdatableSystem {
public:
	AnimationFrequencyCalculatorSystem();

	void update(float updateTime, const sl::Entity& entity);

private:

	RegisterUpdatableSystem();
};