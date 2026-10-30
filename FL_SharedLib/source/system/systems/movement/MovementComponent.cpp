#include "pch.h"
#include "system/systems/movement/MovementComponent.hpp"
#include <cmath>
#include "network/NetUtils.hpp"
#include "system/Utils.hpp"
#include "system/Entity.hpp"

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
	sl::net::write<float>(out, movementEndPoint.x);
	sl::net::write<float>(out, movementEndPoint.y);
}

bool sl::MovementComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }
	currentSpeed = sl::net::read<float>(out, offset);
	maxSpeed = sl::net::read<float>(out, offset);
	movementEndPoint.x = sl::net::read<float>(out, offset);
	movementEndPoint.y = sl::net::read<float>(out, offset);
	return true;
}

uint32_t sl::MovementComponent::getSerializeDataSize() const
{
	return sizeof(float) + sizeof(TypeId) + sizeof(uint32_t) + sizeof(float) + sizeof(float) + sizeof(float);
}

uint32_t sl::MovementComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);
}

void sl::MovementComponent::addVelocity(const sf::Vector2f& velocity)
{
	velocityVector += velocity;
}

void sl::MovementComponent::initialize(sl::ComponentInitContext context) {
	if (!context.js) {
		context.entity.addComponent<sl::MovementComponent>();
		return;
	}
	context.entity.addComponent<sl::MovementComponent>(context.js->value("maxVelocity", 20.0f));
}
