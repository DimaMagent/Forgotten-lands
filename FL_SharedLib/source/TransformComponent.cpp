#include "pch.h"
#include "TransformComponent.hpp"

sl::TransformComponent::TransformComponent(){
	setPosition(0.f, 0.f);
	setRotation(0, 0);
}

sl::TransformComponent::TransformComponent(sf::Vector2f startPosition)
{
	setPosition(startPosition);
	setRotation(0, 0);
}

sl::TransformComponent::TransformComponent(float x, float y)
{
	setPosition(x, y);
	setRotation(0, 0);
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

void sl::TransformComponent::serialize(std::vector<uint8_t>& out) const
{
	sl::net::write_uint32_t(out, TypeId);
	sl::net::write_uint32_t(out, getDeserializeDataSize());
	serializeVector2f(out, position);
	serializeVector2i(out, rotation);
}

bool sl::TransformComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if ( offset + getDeserializeDataSize() > out.size()) { return false; }
	position = deserializeVector2f(out, offset);
	rotation = deserializeVector2i(out, offset);
	return true;
}

uint32_t sl::TransformComponent::getSerializeDataSize() const
{
	return sizeof(sf::Vector2f) + sizeof(sf::Vector2i) + sizeof(TypeId) + sizeof(uint32_t);
}

uint32_t sl::TransformComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);
}

