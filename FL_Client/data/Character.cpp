#include "Character.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"

Character::Character(const RenderComponent& rc, const TransformComponent& tc, const MovementComponent& mc) : Entity(rc, tc),
	movementComponent(std::make_unique<MovementComponent>(mc))
{}

Character::~Character() = default;

void Character::move(float deltaTime)
{
	if (movementComponent) {
		movementComponent->move(deltaTime, transformComponent->getPosition());
	}
}

