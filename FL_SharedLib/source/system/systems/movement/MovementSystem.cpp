#include "pch.h"
#include "system/systems/movement/MovementSystem.hpp"
#include "system/Entity.hpp"
#include "system/systems/movement/MovementComponent.hpp"
#include "system/systems/TransformComponent.hpp"
#include "system/systems/StunComponent.hpp"
#include "system/systems/StateComponent.hpp"
#include "system/IEntityRegistry.hpp"
#include <algorithm>

sl::MovementSystem::MovementSystem(const IEntityRegistry& entityRegistry):
    entityRegistry(entityRegistry)
{
    reusableEntityIdsBuffer = std::vector<sl::EntityId>();
}

void sl::MovementSystem::update(float updateTime, const sl::Entity& entity){
    moving(updateTime, entity, entityRegistry);
}

void sl::MovementSystem::moving(float updateTime, const sl::Entity& entity, const IEntityRegistry& entityRegistry)
{
    sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
    sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
    sl::StateComponent* stateComp = entity.getComponent<sl::StateComponent>();
    sl::StunComponent* stunComp = entity.getComponent<sl::StunComponent>();

    if (!movComp || !trComp || !stateComp || !stunComp) return;

    if (!movComp->isMoving()) {
        movComp->braking(updateTime);
        if (!movComp->isMovementAlreadyReseted) {
            stateComp->setCurrentMovementState(sl::MovementState::None);
            movComp->isMovementAlreadyReseted = true;
        }
        return;
    }

    if (stunComp->hasStun(StunState::Immobilized)) { return; }

    sf::Vector2f delta = movComp->calculateDelta(updateTime);
    sf::Vector2f currentPos = trComp->getPosition();

    movComp->setMovementEndPoint(currentPos + delta);
    trComp->setRotation(movComp->getVelocityDirection());
    stateComp->setCurrentMovementState(sl::MovementState::Walk);
    movComp->isMovementAlreadyReseted = false;
}


sl::systemPtr sl::MovementSystem::initialize(sl::SystemCreatorContext& context) {
    return std::make_unique<sl::MovementSystem>(context.entityRegistry);
}

sl::SystemProcessingPhase sl::MovementSystem::getProcessingPhase() {
	return sl::SystemProcessingPhase::Simulation;
}
