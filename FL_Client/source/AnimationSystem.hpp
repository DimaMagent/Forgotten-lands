#pragma once
#include "AnimationType.hpp"
#include "EntityStorage.hpp"
#include "Delegate.hpp"

namespace sl {
	class Entity;
	class StateComponent;
}

class AnimationSystem{
public:
	AnimationSystem(sl::EntityStorage& entities);
	void onUpdate(float updateTime);

private:
	sl::EntityStorage& entities;

	AnimationType selectAnimationType(const sl::StateComponent& stateComp);
	void updateAnimations(sl::Entity& entity, float updateTime);
};