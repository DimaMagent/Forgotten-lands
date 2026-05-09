#include "pch.hpp"
#include "PlayerManager.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"
#include "World.hpp"

PlayerManager::PlayerManager(World& world):
	world(world)
{
	initPlayerActions();
}

void PlayerManager::updatePlayerInputState(uint32_t playerToken, uint8_t inputState, bool pressingFlag)
{
	if (auto entity = world.getPlayerEntityToToken(playerToken).lock()) {
		if (auto it = PlayerActions.find(sl::net::InputState(inputState)); it != PlayerActions.end()) {
			it->second(*entity, pressingFlag);
		}
	}
}

void PlayerManager::initPlayerActions()
{
	PlayerActions.try_emplace(sl::net::IS_MoveUp, [this](sl::Entity& entity, bool pressingFlag) {
		sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
		if (!movComp) { return; }
		int pressingMultiplier = pressingFlag ? 1 : -1;
		movComp->addDirection(sf::Vector2i(0, -1) * pressingMultiplier);
	});
	PlayerActions.try_emplace(sl::net::IS_MoveLeft, [this](sl::Entity& entity, bool pressingFlag) {
		sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
		if (!movComp) { return; }
		int pressingMultiplier = pressingFlag ? 1 : -1;
		movComp->addDirection(sf::Vector2i(-1, 0) * pressingMultiplier);
	});
	PlayerActions.try_emplace(sl::net::IS_MoveDown, [this](sl::Entity& entity, bool pressingFlag) {
		sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
		if (!movComp) { return; }
		int pressingMultiplier = pressingFlag ? 1 : -1;
		movComp->addDirection(sf::Vector2i(0, 1) * pressingMultiplier);
	});
	PlayerActions.try_emplace(sl::net::IS_MoveRight, [this](sl::Entity& entity, bool pressingFlag) {
		sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
		if (!movComp) { return; }
		int pressingMultiplier = pressingFlag ? 1 : -1;
		movComp->addDirection(sf::Vector2i(1, 0) * pressingMultiplier);
	});
}
