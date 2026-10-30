#include "pch.h"
#include "system/systems/movement/SweptAABB.hpp"
#include <algorithm>
#include <vector>
#include <optional>
#include "system/systems/collision/Aabb.hpp"
#include "system/Entity.hpp"
#include "system/systems/movement/MovementComponent.hpp"
#include "system/systems/TransformComponent.hpp"
#include "system/systems/collision/CollisionComponent.hpp"
#include "system/systems/collision/CollisionCellMap.hpp"
#include "system/IEntityRegistry.hpp"

namespace {
    const float infinity = std::numeric_limits<float>::infinity();

    constexpr float EPSILON = 0.000001f;

    constexpr float SKIN = 0.001f;

    std::vector<sl::EntityId> reusableEntityIdsBuffer;
}

sl::SweptAABB::SweptAABB(const sl::CollisionCellMap& collisionCellMap, const IEntityRegistry& entityRegistry):
    collisionCellMap(collisionCellMap), entityRegistry(entityRegistry){}

void sl::SweptAABB::update(float updateTime, const sl::Entity& entity) {

    sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
    if (!movComp) { return; }

    sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
    if (!trComp) { return; }

    sf::Vector2f movementStartPoint = trComp->getPosition();
    sf::Vector2f movementEndPoint = movComp->getMovementEndPoint();

    sf::Vector2f remainingMovement = movementEndPoint - movementStartPoint;

    sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();

    if (!colisComp) {
        trComp->setPosition(movementEndPoint);
        return;
    }

    AABB entityAABB = colisComp->getAABB();

    while (remainingMovement.lengthSquared() > std::pow(EPSILON, 2)) {
        reusableEntityIdsBuffer.clear();

        if (!collisionCellMap.getNearestEntityIdsToEntity(
            entityAABB,
            movementStartPoint,
            reusableEntityIdsBuffer,
            getSearchDepthByDistance(remainingMovement.length())))
        {
            break;
        }

        sl::SweptResult EarliestCollidedResult = {sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f) , FLT_MAX, false};

        for (auto& entityId : reusableEntityIdsBuffer) {
            auto nearestEntity = entityRegistry.getEntityById(entityId);

            if (!nearestEntity.has_value()) { continue; }

            sl::TransformComponent* anotherTrComp = nearestEntity.value().get().getComponent<sl::TransformComponent>();
            if (!anotherTrComp) { continue; }

            sl::CollisionComponent* anotherColisComp = nearestEntity.value().get().getComponent<sl::CollisionComponent>();
            if (!anotherColisComp) { continue; }

            sl::SweptResult result = movementCheck(
                movementStartPoint,
                movementEndPoint,
                anotherTrComp->getPosition(),
                entityAABB,
                anotherColisComp->getAABB()
                );

            if (result.isCollided && result.collidedTime < EarliestCollidedResult.collidedTime) {
                EarliestCollidedResult = result;
            }
        }

        if (!EarliestCollidedResult.isCollided) { break; }

        remainingMovement = EarliestCollidedResult.remainingMovement;
        movementStartPoint = EarliestCollidedResult.collisionPosition;
        movementEndPoint = movementStartPoint + remainingMovement;
    }

    trComp->setPosition(movementEndPoint);
}

sl::SweptResult sl::SweptAABB::movementCheck(const sf::Vector2f& movementStartPoint, const sf::Vector2f& movementEndPoint,
	const sf::Vector2f& blockerPos, const sl::AABB& movingAABB, const sl::AABB& blockerAABB)
{
    const sl::AABB movingWorldAABB =
        movingAABB.getRelativeAABB(
            movementStartPoint.x,
            movementStartPoint.y);

    const sl::AABB blockerWorldAABB =
        blockerAABB.getRelativeAABB(
            blockerPos.x,
            blockerPos.y);

    // Checking whether objects are already intersecting.
    // Here, the touching of boundaries does not, in itself, count as an intersection.
    const bool initiallyOverlapping = movingWorldAABB.intersects(blockerWorldAABB);

    if (initiallyOverlapping){
        return { movementStartPoint, sf::Vector2f(0.0f,0.0f), 0.0f, true };
    }

    const sf::Vector2f delta = movementEndPoint - movementStartPoint;

    if (delta.x == 0.f && delta.y == 0.f)
    {
        return { movementStartPoint, sf::Vector2f(0.0f,0.0f), 0.0f, false };
    }

    // Obstacle extensions applied to the dimensions of a moving AABB.
    sl::AABB expandedBlockerAABB;

    expandedBlockerAABB.minX = blockerWorldAABB.minX - movingAABB.maxX;

    expandedBlockerAABB.maxX = blockerWorldAABB.maxX - movingAABB.minX;

    expandedBlockerAABB.minY = blockerWorldAABB.minY - movingAABB.maxY;

    expandedBlockerAABB.maxY = blockerWorldAABB.maxY - movingAABB.minY;

    float entryTimeX;
    float exitTimeX;
    float entryTimeY;
    float exitTimeY;



    if (delta.x == 0.f)
    {
        if (movementStartPoint.x < expandedBlockerAABB.minX ||
            movementStartPoint.x > expandedBlockerAABB.maxX)
        {
            return { sf::Vector2f(0.0f,0.0f), sf::Vector2f(0.0f,0.0f), 0.0f, false };;
        }

        entryTimeX = -infinity;
        exitTimeX = infinity;
    }
    else
    {
        const float tx1 = (expandedBlockerAABB.minX - movementStartPoint.x) / delta.x;

        const float tx2 = (expandedBlockerAABB.maxX - movementStartPoint.x) / delta.x;

        entryTimeX = std::min(tx1, tx2);
        exitTimeX = std::max(tx1, tx2);
    }

    if (delta.y == 0.f)
    {
        if (movementStartPoint.y < expandedBlockerAABB.minY ||
            movementStartPoint.y > expandedBlockerAABB.maxY)
        {
            return { sf::Vector2f(0.0f,0.0f), sf::Vector2f(0.0f,0.0f), 0.0f, false };;
        }

        entryTimeY = -infinity;
        exitTimeY = infinity;
    }
    else
    {
        const float ty1 = (expandedBlockerAABB.minY - movementStartPoint.y) / delta.y;

        const float ty2 = (expandedBlockerAABB.maxY - movementStartPoint.y) / delta.y;

        entryTimeY = std::min(ty1, ty2);
        exitTimeY = std::max(ty1, ty2);
    }

    const float entryTime = std::max(entryTimeX, entryTimeY);

    const float exitTime = std::min(exitTimeX, exitTimeY);

    const bool hasCollision =
        entryTime <= exitTime &&
        exitTime >= 0.f &&
        entryTime >= 0.f &&
        entryTime <= 1.f;

    if (!hasCollision)
    {
        return { sf::Vector2f(0.0f,0.0f), sf::Vector2f(0.0f, 0.0f), 0.0f, false };;
    }

    sf::Vector2f normal;

    if (std::abs(entryTimeX - entryTimeY) <= EPSILON)
    {
        normal = { delta.x > 0.f ? -1.f : 1.f, delta.y > 0.f ? -1.f : 1.f };
        
    }
    else if (entryTimeX > entryTimeY) {
        normal = { delta.x > 0 ? -1.f : 1.f, 0.f };
    }
    else
    {
        normal = { 0.f, delta.y > 0 ? -1.f : 1.f };
    }

    const float safeTime = std::max(0.f, entryTime - SKIN / std::max(std::hypot(delta.x, delta.y), SKIN));

    sf::Vector2f collisionPosition = movementStartPoint + delta * safeTime;

    sf::Vector2f remainingMovement = delta * (1.f - safeTime);

    if (normal.x != 0.f)
        remainingMovement.x = 0.f;

    if (normal.y != 0.f)
        remainingMovement.y = 0.f;

    return { collisionPosition, remainingMovement, entryTime, true };
}

sl::systemPtr sl::SweptAABB::initialize(sl::SystemCreatorContext& context) {
    return std::make_unique<sl::SweptAABB>(context.collisionCellMap, context.entityRegistry);
}

sl::SystemProcessingPhase sl::SweptAABB::getProcessingPhase() {
    return sl::SystemProcessingPhase::PostSimulation;
}