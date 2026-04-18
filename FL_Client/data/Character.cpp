#include "Character.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include <iostream>


Character::Character(const RenderComponent& rc, const TransformComponent& tc, const MovementComponent& mc) : Entity(rc, tc),
	movementComponent(std::make_unique<MovementComponent>(mc))
{}

Character::Character(const Character & other) : Entity(*other.renderComponent, *other.transformComponent),
	movementComponent(std::make_unique<MovementComponent>(*other.movementComponent))
{}

Character::Character(Character && other) noexcept : Entity(*other.renderComponent, *other.transformComponent),
movementComponent(std::move(other.movementComponent))
{}

Character::~Character() = default;

void Character::move(float deltaTime)
{
	if (movementComponent) {
		transformComponent->setPosition(movementComponent->move(deltaTime, transformComponent->getPosition()));
	}
	else {
		std::cout << "Character has no movement component!" << std::endl;
	}
}
void Character::addVelocityVector(sf::Vector2i direction, float speed) {
	if (movementComponent) {
		movementComponent->addVelocityVector(direction, speed);
	}
	else {
		std::cout << "Character has no movement component!" << std::endl;
	}
}

void Character::addDirection(sf::Vector2i direction) {
	if (movementComponent) {
		movementComponent->addDirection(direction);
	}
	else {
		std::cout << "Character has no movement component!" << std::endl;
	}
}