#include "MovementComponent.hpp"
#include <cmath>

MovementComponent::MovementComponent(float maxSpeed, sf::Time maxAccelerationTime) : maxSpeed(maxSpeed), maxAccelerationTime(maxAccelerationTime)
{
}

void MovementComponent::addMovement(sf::Vector2i direction)
{
	addVelocity(asNormalized(sf::Vector2f(direction)) * CurrentSpeed);
	isAccelerating = true;
}

void MovementComponent::move(float deltaTime, sf::Vector2f& position)
{
	if (isAccelerating) {
		currentAccelerationTime > maxAccelerationTime ? currentAccelerationTime = maxAccelerationTime :
			currentAccelerationTime += sf::seconds(deltaTime);
		float t = currentAccelerationTime.asSeconds() / maxAccelerationTime.asSeconds();
		CurrentSpeed = maxSpeed * (3 * t * t - 2 * t * t * t);
	}
	else {
		currentAccelerationTime = sf::Time::Zero;
		CurrentSpeed = 0.f;
	}
	position += velocityVector * deltaTime;
	resetVelocity();
	isAccelerating = false;
}


void MovementComponent::addVelocity(const sf::Vector2f& velocity)
{
	velocityVector += velocity;
}

sf::Vector2f MovementComponent::asNormalized(const sf::Vector2f& vector) const
{
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length > 0.0001f) {
		return sf::Vector2f(vector.x / length, vector.y / length);
	}
	else {
		return sf::Vector2f(0.f, 0.f);
	}
}
