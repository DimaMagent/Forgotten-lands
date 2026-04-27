#include "pch.h"
#include "MovementComponent.hpp"
#include <cmath>

sl::MovementComponent::MovementComponent(float maxSpeed, sf::Time maxAccelerationTime) : maxSpeed(maxSpeed), maxAccelerationTime(maxAccelerationTime)
{
}
void sl::MovementComponent::addVelocityVector(sf::Vector2i direction, float speed)
{
	addVelocity(asNormalized(sf::Vector2f(direction)) * speed);
}

void sl::MovementComponent::addDirection(sf::Vector2i direction)
{
	directionVector = inBounds(asNormalized(direction) + directionVector, sf::Vector2i(-1, -1), sf::Vector2i(1, 1));
}

sf::Vector2f sl::MovementComponent::move(float deltaTime, const sf::Vector2f& position)
{
	if (directionVector != sf::Vector2i(0, 0)) {
		currentAccelerationTime > maxAccelerationTime ? currentAccelerationTime = maxAccelerationTime :
			currentAccelerationTime += std::min(sf::seconds(deltaTime), maxAccelerationTime);
		float t = currentAccelerationTime.asSeconds() / maxAccelerationTime.asSeconds();
		currentSpeed = maxSpeed * (3 * t * t - 2 * t * t * t);
	}
	else if (currentSpeed > 0.f){
		currentAccelerationTime = std::max(sf::Time::Zero, currentAccelerationTime - sf::seconds(deltaTime));
		float t = currentAccelerationTime.asSeconds() / maxAccelerationTime.asSeconds();
		currentSpeed = maxSpeed * (3 * t * t - 2 * t * t * t);
	}
	if (currentSpeed > 0.f) {
		if (directionVector == sf::Vector2i(1, 1) || directionVector == sf::Vector2i(-1, 1) || directionVector == sf::Vector2i(1, -1) || directionVector == sf::Vector2i(-1, -1)) {
			currentSpeed /= std::sqrt(directionVector.x * directionVector.x + directionVector.y * directionVector.y);
		}
		velocityVector += sf::Vector2f(directionVector) * currentSpeed * deltaTime;
	}
	sf::Vector2f newPosition = position + velocityVector;
	resetVelocity();
	return newPosition;
}

void sl::MovementComponent::addVelocity(const sf::Vector2f& velocity)
{
	velocityVector += velocity;
}

sf::Vector2f sl::MovementComponent::asNormalized(const sf::Vector2f& vector) const
{
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length > 0.0001f) {
		return sf::Vector2f(vector.x / length, vector.y / length);
	}
	else {
		return sf::Vector2f(0.f, 0.f);
	}
}

sf::Vector2i sl::MovementComponent::asNormalized(const sf::Vector2i& vector) const
{
	double length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length >= 0.0001) {
		return sf::Vector2i(std::round(vector.x / length), std::round(vector.y / length));
	}
	else {
		return sf::Vector2i(0, 0);
	}
}

sf::Vector2i sl::MovementComponent::inBounds(const sf::Vector2i& vector, const sf::Vector2i& minBounds, const sf::Vector2i& maxBounds) const
{
	sf::Vector2i boundedVector = vector;
	if (vector.x <= minBounds.x) {
		boundedVector.x = minBounds.x;
	}
	else if (vector.x >= maxBounds.x) {
		boundedVector.x = maxBounds.x;
	}

	if (vector.y <= minBounds.y) {
		boundedVector.y = minBounds.y;
	}
	else if (vector.y >= maxBounds.y) {
		boundedVector.y = maxBounds.y;
	}
	return boundedVector;
}
