#include "pch.h"
#include "Entity.hpp"
#include "MovementSystem.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "StateComponent.hpp"
#include "CollisionSystem.hpp"

sl::MovementSystem::MovementSystem(CollisionSystem& collisionSystem): collisionSystem(collisionSystem)
{
}

void sl::MovementSystem::onUpdate(sl::Entity& entity, float updateTime) const{
    movingWithCollisionCheck(entity, updateTime);
}

void sl::MovementSystem::movingWithCollisionCheck(sl::Entity& entity, float updateTime) const
{
    sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
    sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
    sl::StateComponent* stateComp = entity.getComponent<sl::StateComponent>();

    if (!movComp || !trComp || !stateComp) return;

    if (!movComp->isMoving()) {
        movComp->braking(updateTime);
        if (!movComp->isMovementAlreadyReseted) {
            stateComp->setCurrentMovementState(sl::MovementState::None);
            movComp->isMovementAlreadyReseted = true;
        }
        return;
    }

    sf::Vector2f delta = movComp->calculateDelta(updateTime);
    sf::Vector2f currentPos = trComp->getPosition();

    if (delta.x != 0.f) {
        sf::Vector2f targetPosX = { currentPos.x + delta.x, currentPos.y };
        if (!collisionSystem.isBlockedOnPosition(entity, targetPosX)) {
            currentPos.x = targetPosX.x;
        }
    }

    if (delta.y != 0.f) {
        sf::Vector2f targetPosY = { currentPos.x, currentPos.y + delta.y };
        if (!collisionSystem.isBlockedOnPosition(entity, targetPosY)) {
            currentPos.y = targetPosY.y;
        }
    }

    trComp->setPosition(currentPos);
    trComp->setRotation(movComp->getVelocityDirection());
    stateComp->setCurrentMovementState(sl::MovementState::Walk);
    movComp->isMovementAlreadyReseted = false;
}


