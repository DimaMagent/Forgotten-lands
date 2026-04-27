#include "pch.h"
#include "TransformComponent.hpp"


sl::TransformComponent::TransformComponent()
{ 
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
