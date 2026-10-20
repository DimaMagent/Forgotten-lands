#pragma once
#include "system/systems/SystemUpdater.hpp"

class LocalWorld;
class AnimationSystem;

class ClientSystemUpdater : public sl::SystemUpdater {
public:
	ClientSystemUpdater(const sl::SystemCreatorContext& context);

protected:

	virtual void onUpdate(float deltaTime) override;

	virtual void onEnitiesUpdate(float deltaTime, const sl::Entity& entity) override;

};