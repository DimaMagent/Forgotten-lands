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
}

void sl::MovementSystem::update(float updateTime, const sl::Entity& entity){
    moving(updateTime, entity, entityRegistry);
}

sf::Vector2f sl::MovementSystem::calculateDelta(float deltaTime, sl::MovementComponent& movComp) {

    sf::Vector2i velocityDirectionVector = movComp.getVelocityDirection();
    const sf::Time& maxAccelerationTime = movComp.getMaxAccelerationTime();

    if (velocityDirectionVector != sf::Vector2i(0, 0)) {
        movComp.getCurrentAccelerationTime() > maxAccelerationTime
            ? movComp.setCurrentAccelerationTime(maxAccelerationTime)
            : movComp.setCurrentAccelerationTime(std::min(sf::seconds(deltaTime) + movComp.getCurrentAccelerationTime(), maxAccelerationTime));

        float t = movComp.getCurrentAccelerationTime().asSeconds() / maxAccelerationTime.asSeconds();
        movComp.setCurrentSpeed(movComp.getMaxSpeed() * (3 * t * t - 2 * t * t * t));
    }

    if (movComp.getCurrentSpeed() > 0.f) {
        if (velocityDirectionVector == sf::Vector2i(1, 1) || velocityDirectionVector == sf::Vector2i(-1, 1) ||
            velocityDirectionVector == sf::Vector2i(1, -1) || velocityDirectionVector == sf::Vector2i(-1, -1)) {
            movComp.setCurrentSpeed(movComp.getCurrentSpeed() / std::sqrt(velocityDirectionVector.x * velocityDirectionVector.x + velocityDirectionVector.y * velocityDirectionVector.y));
        }
       movComp.addVelocity(sf::Vector2f(velocityDirectionVector) * movComp.getCurrentSpeed() * deltaTime);
    }

    sf::Vector2f delta = movComp.getVelocity();
    movComp.resetVelocity();
    return delta;
}

void sl::MovementSystem::braking(float deltaTime, sl::MovementComponent& movComp) {
    if (movComp.getCurrentSpeed() > 0.f) {
        movComp.setCurrentAccelerationTime(std::max(sf::Time::Zero, movComp.getCurrentAccelerationTime() - sf::seconds(deltaTime)));
        float t = movComp.getCurrentAccelerationTime().asSeconds() / movComp.getMaxAccelerationTime().asSeconds();
        movComp.setCurrentSpeed(movComp.getMaxSpeed() * (3 * t * t - 2 * t * t * t));
    }
}

void sl::MovementSystem::moving(float updateTime, const sl::Entity& entity, const IEntityRegistry& entityRegistry)
{
    sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
    sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
    sl::StateComponent* stateComp = entity.getComponent<sl::StateComponent>();
    sl::StunComponent* stunComp = entity.getComponent<sl::StunComponent>();

    if (!movComp || !trComp || !stateComp || !stunComp) return;

    if (!movComp->isMoving()) {
        braking(updateTime, *movComp);
        if (!movComp->isMovementAlreadyReseted) {
            stateComp->setCurrentMovementState(sl::MovementState::None);
            movComp->isMovementAlreadyReseted = true;
        }
        return;
    }

    if (stunComp->hasStun(StunState::Immobilized)) { return; }

    sf::Vector2f delta = calculateDelta(updateTime, *movComp);
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


