#include "pch.h"
#include "MovementComponent.hpp"
#include <cmath>
#include "NetUtils.hpp"
#include "Utils.hpp"


sl::MovementComponent::MovementComponent(float maxSpeed, sf::Time maxAccelerationTime) : maxSpeed(maxSpeed), maxAccelerationTime(maxAccelerationTime)
{}

void sl::MovementComponent::addVelocityVector(sf::Vector2i direction, float speed)
{
	addVelocity(sl::asNormalized(sf::Vector2f(direction)) * speed);
}

void sl::MovementComponent::addDirection(sf::Vector2i direction)
{
	velocityDirectionVector = sl::inBounds(sl::asNormalized(direction) + velocityDirectionVector, sf::Vector2i(-1, -1), sf::Vector2i(1, 1));
}

sf::Vector2f sl::MovementComponent::calculateDelta(float deltaTime)
{
	if (velocityDirectionVector != sf::Vector2i(0, 0)) {
		currentAccelerationTime > maxAccelerationTime
			? currentAccelerationTime = maxAccelerationTime
			: currentAccelerationTime = std::min(sf::seconds(deltaTime) + currentAccelerationTime, maxAccelerationTime);

		float t = currentAccelerationTime.asSeconds() / maxAccelerationTime.asSeconds();
		currentSpeed = maxSpeed * (3 * t * t - 2 * t * t * t);
	}

	if (currentSpeed > 0.f) {
		if (velocityDirectionVector == sf::Vector2i(1, 1) || velocityDirectionVector == sf::Vector2i(-1, 1) ||
			velocityDirectionVector == sf::Vector2i(1, -1) || velocityDirectionVector == sf::Vector2i(-1, -1)) {
			currentSpeed /= std::sqrt(velocityDirectionVector.x * velocityDirectionVector.x + velocityDirectionVector.y * velocityDirectionVector.y);
		}
		velocityVector += sf::Vector2f(velocityDirectionVector) * currentSpeed * deltaTime;
	}

	sf::Vector2f delta = velocityVector;
	resetVelocity();
	return delta;
}

void sl::MovementComponent::braking(float deltaTime)
{
	if (currentSpeed > 0.f) {
		currentAccelerationTime = std::max(sf::Time::Zero, currentAccelerationTime - sf::seconds(deltaTime));
		float t = currentAccelerationTime.asSeconds() / maxAccelerationTime.asSeconds();
		currentSpeed = maxSpeed * (3 * t * t - 2 * t * t * t);
	}
}

void sl::MovementComponent::setVelocityDirection(const sf::Vector2i& direction)
{

	velocityDirectionVector = sl::inBounds(direction, sf::Vector2i(-1, -1), sf::Vector2i(1, 1));
}

void sl::MovementComponent::serialize(std::vector<uint8_t>& out) const
{
	sl::net::write<uint32_t>(out, TypeId);
	sl::net::write<uint32_t>(out, getDeserializeDataSize());
	sl::net::write<float>(out, currentSpeed);
	sl::net::write<float>(out, maxSpeed);
}

bool sl::MovementComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }
	currentSpeed = sl::net::read<float>(out, offset);
	maxSpeed = sl::net::read<float>(out, offset);
	return true;
}

uint32_t sl::MovementComponent::getSerializeDataSize() const
{
	return sizeof(float) + sizeof(TypeId) + sizeof(uint32_t) + sizeof(float);
}

uint32_t sl::MovementComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);
}

void sl::MovementComponent::addVelocity(const sf::Vector2f& velocity)
{
	velocityVector += velocity;
}
