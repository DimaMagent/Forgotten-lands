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
	AnimationSystem(std::weak_ptr<sl::Entity> playerEntity, sl::EntityStorage& entities,
		sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate);
	void onUpdate(sl::Entity& entity, float updateTime);

private:
	static int serializationFrequency;
	int serializationCounter = 0;

	std::weak_ptr<sl::Entity> playerEntity;
	sl::EntityStorage& entities;

	AnimationType selectAnimationType(const sl::StateComponent& stateComp);
	void updateAnimations(sl::Entity& entity, float updateTime);
};