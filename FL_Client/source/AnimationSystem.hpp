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
	AnimationSystem();
	void onUpdate(float updateTime, const sl::Entity& entity);

private:
	AnimationType selectAnimationType(const sl::StateComponent& stateComp);
	AnimationType selectInstantAnimationType(const sl::StateComponent& stateComp);
	bool isInstantAnimation(AnimationType type);
	void updateAnimations(const sl::Entity& entity, float updateTime);
};