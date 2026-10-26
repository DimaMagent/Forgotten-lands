#pragma once
#include "system/systems/UpdatableSystem.hpp"

class IPlayerEntityStorage;

class IntentSenderSystem: public sl::UpdatableSystem {
public:
	IntentSenderSystem(const IPlayerEntityStorage& playerEntityStorage);

	void update(float updateTime, const sl::Entity& entity) override;

private:
	const IPlayerEntityStorage& playerEntityStorage;

	RegisterUpdatableSystem()
};