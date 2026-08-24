#include "pch.h"
#include "AttackSystem.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <optional>
#include "WeaponComponent.hpp"
#include "TransformComponent.hpp"
#include "CollisionComponent.hpp"
#include "HealthComponent.hpp"
#include "StateComponent.hpp"
#include "Aabb.hpp"
#include "Entity.hpp"
#include "CollisionCellMap.hpp"
#include "WorldBase.hpp"
#include "WorldMap.hpp"



sl::AttackSystem::AttackSystem()
{
    reusableEntityIdsBuffer = std::vector<uint32_t>();
    temporaryIgnoreList = std::set<uint32_t>();
}

bool sl::AttackSystem::tryMeleeAttack(sl::Entity& attackingEntity, const WorldBase& world) {
	sl::WeaponComponent* weaponComp = attackingEntity.getComponent<sl::WeaponComponent>();
	if (!weaponComp) { return false; }

	sl::TransformComponent* transComp = attackingEntity.getComponent<sl::TransformComponent>();
	if (!transComp) { return false; }

    sl::CollisionComponent* colisComp = attackingEntity.getComponent<sl::CollisionComponent>();
    if (!colisComp) { return false; }

    const auto WorldMap = world.getWorldMap();

    if (!WorldMap.has_value()) { return false; }

    const auto collisionCellMap = WorldMap.value().get().getCollisionMap();

    if (!collisionCellMap.has_value()) { return false; }

    AABB aabb = colisComp->getAABB();
	float attackDistance = weaponComp->getAttackDistance();
    float attackDegrees = weaponComp->getAttackDegrees();
	sf::Vector2f entityPos = transComp->getPosition();
	sf::Vector2i entityRotation = transComp->getRotation();

    bool isSuccess = collisionCellMap.value().get().getNearestEntityIdsToEntity(
        aabb,
        entityPos,
        reusableEntityIdsBuffer,
        sl::CollisionCellMap::getSearchDepthByDistance(attackDistance)
    );

    if (!isSuccess) { return false; }

    sl::StateComponent* stateComp = attackingEntity.getComponent<sl::StateComponent>();

    if (stateComp)
    {
        stateComp->setCurrentActionState(sl::ActionState::MeleeAttack);
    }

    for (uint32_t id : reusableEntityIdsBuffer) {

        auto entityOpt = world.getEntityById(id);
        if (!entityOpt.has_value()) { continue; }

        if (id == attackingEntity.getGlobalId()) { continue; }

        auto it = temporaryIgnoreList.find(id);

        if (it != temporaryIgnoreList.end()) { continue; }

        sl::CollisionComponent* anotherColisComp = entityOpt.value().get().getComponent<sl::CollisionComponent>();
        if (!anotherColisComp) { continue; }

        sl::TransformComponent* anotherTransComp = entityOpt.value().get().getComponent<sl::TransformComponent>();
        if (!anotherTransComp) { continue; }

        sl::HealthComponent* anotherHealthComp = entityOpt.value().get().getComponent<sl::HealthComponent>();
        if (!anotherHealthComp) { continue; }

        AABB otherAABB = anotherColisComp->getAABB();

        sf::Vector2f otherPosition = anotherTransComp->getPosition();
        
        bool isAttackSuccess = isAABBinAttackCone(
            entityPos,
            static_cast<sf::Vector2f>(entityRotation),
            otherAABB,
            otherPosition,
            attackDistance,
            attackDegrees
        );

        if (isAttackSuccess) {
            temporaryIgnoreList.emplace(id);
            anotherHealthComp->takeDamage(weaponComp->getAttackDamage());
            std::cout << "Entity with id: " << attackingEntity.getGlobalId() << " attacked entity with id " << id
                << "currentHealth " << anotherHealthComp->getCurrentHealth() << "\n";
        }
    }
    temporaryIgnoreList.clear();
    reusableEntityIdsBuffer.clear();

    return true;
}

bool sl::AttackSystem::isAABBinAttackCone(
    sf::Vector2f attackerPos,
    sf::Vector2f lookDir,
    const sl::AABB& targetAABB,
    sf::Vector2f targetPos,
    float attackRadius,
    float maxAngleDegrees)
{
    float lookLen = std::sqrt(lookDir.x * lookDir.x + lookDir.y * lookDir.y);
    if (lookLen < 0.0001f) return false;
    sf::Vector2f lookDirNorm = lookDir / lookLen;

    sl::AABB worldAABB(targetAABB.minX + targetPos.x, targetAABB.minY + targetPos.y,
        targetAABB.maxX + targetPos.x, targetAABB.maxY + targetPos.y);

    sf::Vector2f closestPoint(
        std::clamp(attackerPos.x, worldAABB.minX, worldAABB.maxX),
        std::clamp(attackerPos.y, worldAABB.minY, worldAABB.maxY)
    );

    sf::Vector2f toTarget = closestPoint - attackerPos;
    float distSq = toTarget.x * toTarget.x + toTarget.y * toTarget.y;

    float radiusSq = attackRadius * attackRadius;
    if (distSq > radiusSq) return false;

    if (distSq < 0.0001f) return true;

    float dist = std::sqrt(distSq);
    sf::Vector2f toTargetNorm = toTarget / dist;

    float dot = lookDirNorm.x * toTargetNorm.x + lookDirNorm.y * toTargetNorm.y;
    float minDotCos = std::cos(maxAngleDegrees * 3.14159265f / 180.0f);

    return dot >= minDotCos;
}

void sl::AttackSystem::attackEnd(sl::Entity& attackingEntity) {
    sl::StateComponent* stateComp = attackingEntity.getComponent<sl::StateComponent>();

    if (stateComp)
    {
        stateComp->setCurrentActionState(sl::ActionState::None);
    }
}