#include "pch.h"
#include "TransformComponent.hpp"
#include "Utils.hpp"

sl::TransformComponent::TransformComponent(){
	setPosition(0.f, 0.f);
	setRotation(0, -1);
}

sl::TransformComponent::TransformComponent(sf::Vector2f startPosition)
{
	setPosition(startPosition);
	setRotation(0, -1);
}

sl::TransformComponent::TransformComponent(float x, float y)
{
	setPosition(x, y);
	setRotation(0, -1);
}

void sl::TransformComponent::setPosition(float x, float y)
{
	bool isCellChanged = (static_cast<int>(position.x / Cell::getCellSize()) != static_cast<int>(x / Cell::getCellSize())) ||
		(static_cast<int>(position.y / Cell::getCellSize()) != static_cast<int>(y / Cell::getCellSize()));

	position.x = x;
	position.y = y;

	if (isCellChanged) {
		onCellChanged.broadcast(position);
	}
}

void sl::TransformComponent::setPosition(const sf::Vector2f& position)
{
	setPosition(position.x, position.y);
}

void sl::TransformComponent::setRotation(int x, int y)
{
	if (x == 0 && y == 0) { return; }

	rotation = sl::inBounds(sf::Vector2i(x, y), sf::Vector2i(-1, -1), sf::Vector2i(1, 1));
}

void sl::TransformComponent::setRotation(const sf::Vector2i& rotation)
{
	if (rotation.x == 0 && rotation.y == 0) { return; }

	this->rotation = sl::inBounds(rotation, sf::Vector2i(-1, -1), sf::Vector2i(1, 1));
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
	setRotation(deserializeVector2i(out, offset));
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

