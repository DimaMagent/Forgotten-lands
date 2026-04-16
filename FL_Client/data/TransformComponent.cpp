#include "TransformComponent.hpp"

TransformComponent::TransformComponent()
{}

TransformComponent::TransformComponent(sf::Vector2f startPosition)
{
	setPosition(startPosition);	
}

TransformComponent::TransformComponent(float x, float y)
{
	setPosition(x, y);
}

void TransformComponent::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void TransformComponent::setPosition(const sf::Vector2f& position)
{
	this->position = position;
}

sf::Vector2f& TransformComponent::getPosition()
{
	return position;
}
