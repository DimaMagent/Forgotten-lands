#pragma once
#include "AnimationType.hpp"
#include "system/storages/EntityStorage.hpp"
#include "system/systems/UpdatableSystem.hpp"

namespace sl {
	class Entity;
	class StateComponent;
}

class AnimationSystem : public sl::UpdatableSystem {
public:
	AnimationSystem();
	void update(float updateTime, const sl::Entity& entity);

private:
	AnimationType selectAnimationType(const sl::StateComponent& stateComp);
	AnimationType selectInstantAnimationType(const sl::StateComponent& stateComp);
	bool isInstantAnimation(AnimationType type);
	void updateAnimations(const sl::Entity& entity, float updateTime);

	RegisterUpdatableSystem();
};