#pragma once
#include "SystemUpdater.hpp"

class LocalWorld;
class AnimationSystem;

class ClientSystemUpdater : public sl::SystemUpdater {
public:
	ClientSystemUpdater();

protected:
	std::unique_ptr<AnimationSystem> animationSystem;

	virtual void onUpdate(float deltaTime, const sl::WorldBase& world) override;

	virtual void onEnitiesUpdate(float deltaTime, const sl::WorldBase& world, const sl::Entity& entity) override;

};