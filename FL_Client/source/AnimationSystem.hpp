#pragma once
#include "AnimationType.hpp"

namespace sl {
	class Entity;
	class StateComponent;
}

class AnimationSystem{
public:
	AnimationSystem() = default;
	void onUpdate(sl::Entity& entity, float updateTime);

private:
	static int serializationFrequency;
	int serializationCounter = 0;

	AnimationType selectAnimationType(const sl::StateComponent& stateComp);
};