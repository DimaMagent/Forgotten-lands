#include "pch.h"
#include "Entity.hpp"
#include "MovementSystem.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "CollisionComponent.hpp"
#include "StateComponent.hpp"
#include "CollisionCellMap.hpp"
#include "WorldBase.hpp"
#include <algorithm>

sl::MovementSystem::MovementSystem()
{
    reusableEntityIdsBuffer = std::vector<uint32_t>();
}

void sl::MovementSystem::onUpdate(float updateTime, sl::Entity& entity, const sl::CollisionCellMap& collisionCellMap, const WorldBase& world){
    movingWithCollisionCheck(updateTime, entity,collisionCellMap, world);
}

void sl::MovementSystem::movingWithCollisionCheck(float updateTime, sl::Entity& entity,
    const sl::CollisionCellMap& collisionCellMap, const WorldBase& world)
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
        standartPositionCalculate(entity, delta, currentPos, collisionCellMap, world);
    }
    else {
        subSteppingPositionCalculate(entity, delta, currentPos, collisionCellMap, world);
    }

    trComp->setPosition(currentPos);
    trComp->setRotation(movComp->getVelocityDirection());
    stateComp->setCurrentMovementState(sl::MovementState::Walk);
    movComp->isMovementAlreadyReseted = false;
}

void sl::MovementSystem::standartPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos,
    const sl::CollisionCellMap& collisionCellMap, const WorldBase& world)
{
    if (delta.x != 0.f) {
        sf::Vector2f targetPosX = { currentPos.x + delta.x, currentPos.y };
        if (!isBlockedOnPosition(entity, targetPosX, collisionCellMap, world)) {
            currentPos.x = targetPosX.x;
        }
    }

    if (delta.y != 0.f) {
        sf::Vector2f targetPosY = { currentPos.x, currentPos.y + delta.y };
        if (!isBlockedOnPosition(entity, targetPosY, collisionCellMap, world)) {
            currentPos.y = targetPosY.y;
        }
    }
}

void sl::MovementSystem::subSteppingPositionCalculate(sl::Entity& entity, const sf::Vector2f& delta, sf::Vector2f& currentPos,
    const sl::CollisionCellMap& collisionCellMap, const WorldBase& world)
{
    float maxStepSize = MAX_STEP_SIZE;

    float totalDistance = std::hypot(delta.x, delta.y);
    int steps = std::max(1, static_cast<int>(std::ceil(totalDistance / maxStepSize)));

    sf::Vector2f subDelta = delta / static_cast<float>(steps);

    for (int i = 0; i < steps; ++i) {

        if (subDelta.x != 0.f) {
            sf::Vector2f targetPosX = { currentPos.x + subDelta.x, currentPos.y };
            if (!isBlockedOnPosition(entity, targetPosX, collisionCellMap, world)) {
                currentPos.x = targetPosX.x;
            }
            else {
                subDelta.x = 0.f;
            }
        }

        if (subDelta.y != 0.f) {
            sf::Vector2f targetPosY = { currentPos.x, currentPos.y + subDelta.y };
            if (!isBlockedOnPosition(entity, targetPosY, collisionCellMap, world)) {
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

bool sl::MovementSystem::isBlockedOnPosition(sl::Entity& entity, const sf::Vector2f& testPos,
    const sl::CollisionCellMap& collisionCellMap, const WorldBase& world)
{
    sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
    if (!colisComp) { return false; }

    AABB aabb = colisComp->getAABB();
    reusableEntityIdsBuffer.clear();

    if (!collisionCellMap.getNearestEntityIdsToEntity(aabb, testPos, reusableEntityIdsBuffer, ON_POSITION_SEARCH_DEPTH)) {
        return false;
    }

    for (uint32_t id : reusableEntityIdsBuffer) {
        if (id == entity.getGlobalId()) continue;

        auto entityOpt = world.getEntityById(id);
        if (!entityOpt.has_value()) continue;

        sl::CollisionComponent* anotherColisComp = entityOpt.value().get().getComponent<sl::CollisionComponent>();
        sl::TransformComponent* anotherTransComp = entityOpt.value().get().getComponent<sl::TransformComponent>();
        if (!anotherColisComp || !anotherTransComp) continue;

        if (anotherColisComp->getCollisionType() != CollisionType::Block) continue;

        AABB otherAABB = anotherColisComp->getAABB();
        sf::Vector2f otherPosition = anotherTransComp->getPosition();

        sl::CollisionType type = colisComp->isRelativeCollisionWith(testPos.x, testPos.y, otherAABB, otherPosition.x, otherPosition.y);

        if (type == sl::CollisionType::Block) {
            return true;
        }
    }

    return false;
}
