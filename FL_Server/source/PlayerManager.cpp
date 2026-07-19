#include "pch.hpp"
#include "PlayerManager.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include "Utils.hpp"

PlayerManager::PlayerManager(World& world):
	world(world)
{
	initPlayerActions();
}

void PlayerManager::updatePlayerInputState(uint32_t playerToken, sf::Vector2i movementDirectionIntentions, uint8_t inputState, uint8_t inputAction)
{

	auto entity = world.getPlayerEntityToToken(playerToken).lock();
	if (!entity) { return; }

	sl::MovementComponent* movComp = entity->getComponent<sl::MovementComponent>();
	if (!movComp) { return; }

	movComp->setVelocityDirection(sl::inBounds(movementDirectionIntentions, sf::Vector2i(-1, -1), sf::Vector2i(1, 1)));
}

void PlayerManager::initPlayerActions()
{
}

void PlayerManager::movementUpdate(sl::Entity& entity, sf::Vector2i movementDirectionIntentions)
{
	

}
