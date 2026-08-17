#include "pch.h"
#include "Entity.hpp"
#include "MovementSystem.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "StateComponent.hpp"
#include "CollisionSystem.hpp"
#include <algorithm>

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

    if (delta.x < MIN_SPEED_FOR_SUB_STEPPING_ALGORITHM && delta.y < MIN_SPEED_FOR_SUB_STEPPING_ALGORITHM) {
        std::cout << "standart algorithm\n";
        standartPositionCalculate(entity, delta, currentPos);
    }
    else {
        std::cout << "subStepping algorithm\n";
        subSteppingPositionCalculate(entity, delta, currentPos);
    }

    trComp->setPosition(currentPos);
    trComp->setRotation(movComp->getVelocityDirection());
    stateComp->setCurrentMovementState(sl::MovementState::Walk);
    movComp->isMovementAlreadyReseted = false;
}

void sl::MovementSystem::standartPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos) const
{
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
}

void sl::MovementSystem::subSteppingPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos) const
{
    float maxStepSize = MAX_STEP_SIZE;

    float totalDistance = std::hypot(delta.x, delta.y);
    int steps = std::max(1, static_cast<int>(std::ceil(totalDistance / maxStepSize)));

    sf::Vector2f subDelta = delta / static_cast<float>(steps);

    for (int i = 0; i < steps; ++i) {

        if (subDelta.x != 0.f) {
            sf::Vector2f targetPosX = { currentPos.x + subDelta.x, currentPos.y };
            if (!collisionSystem.isBlockedOnPosition(entity, targetPosX)) {
                currentPos.x = targetPosX.x;
            }
            else {
                subDelta.x = 0.f;
            }
        }

        if (subDelta.y != 0.f) {
            sf::Vector2f targetPosY = { currentPos.x, currentPos.y + subDelta.y };
            if (!collisionSystem.isBlockedOnPosition(entity, targetPosY)) {
                currentPos.y = targetPosY.y;
            }
            else {
                subDelta.y = 0.f;
            }
        }

        if (subDelta.x == 0.f && subDelta.y == 0.f) {
            break;
        }
    }
}


