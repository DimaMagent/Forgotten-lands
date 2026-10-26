#pragma once
#include "system/systems/SystemUpdater.hpp"

class IPlayerEntityStorage;

struct ClientSystemCreatorContext : public sl::SystemCreatorContext {
	ClientSystemCreatorContext(const sl::CollisionCellMap& collisionCellMap, const sl::IEntityRegistry& entityRegistry, const IPlayerEntityStorage& playerEntityStorage) :
		SystemCreatorContext(collisionCellMap, entityRegistry), playerEntityStorage(playerEntityStorage) {
	}
	const IPlayerEntityStorage& playerEntityStorage;

};