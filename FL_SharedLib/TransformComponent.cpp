#include "pch.h"
#include "TransformComponent.hpp"

sl::TransformComponent::TransformComponent(){
	setPosition(0.f, 0.f);
}

sl::TransformComponent::TransformComponent(sf::Vector2f startPosition)
{
	setPosition(startPosition);	
}

sl::TransformComponent::TransformComponent(float x, float y)
{
	setPosition(x, y);
}

void sl::TransformComponent::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void sl::TransformComponent::setPosition(const sf::Vector2f& position)
{
	this->position = position;
}

sf::Vector2f& sl::TransformComponent::getPosition()
{
	return position;
}

void sl::TransformComponent::serialize(std::vector<uint8_t>& out) const
{
	sl::net::write_uint32_t(out, TypeId);
	serializeVector2f(out, position);
}

bool sl::TransformComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if ( offset + getDeserializeDataSize() > out.size()) { return false; }
	position = deserializeVector2f(out, offset);
	return true;
}

uint32_t sl::TransformComponent::getSerializeDataSize() const
{
	return sizeof(sf::Vector2f) + sizeof(TypeId);
}

uint32_t sl::TransformComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId);
}

