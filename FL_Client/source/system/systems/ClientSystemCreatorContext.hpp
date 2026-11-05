#pragma once
#include "system/systems/SystemUpdater.hpp"

class IPlayerEntityStorage;

namespace sf {
	class RenderTarget;
}

struct ClientSystemCreatorContext : public sl::SystemCreatorContext {
	ClientSystemCreatorContext(
		const sl::CollisionGrid& collisionCellMap,
		const sl::IEntityRegistry& entityRegistry,
		const IPlayerEntityStorage& playerEntityStorage,
		sf::RenderTarget& renderTarget):
		SystemCreatorContext(collisionCellMap, entityRegistry),
		playerEntityStorage(playerEntityStorage), renderTarget(renderTarget)
	{
	}
	const IPlayerEntityStorage& playerEntityStorage;
	sf::RenderTarget& renderTarget;
};