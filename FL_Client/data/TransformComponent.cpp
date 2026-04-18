#include "TransformComponent.hpp"
#include <iostream>

TransformComponent::TransformComponent()
{ 
	setPosition(0.f, 0.f);
}

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
	//std::cout << "Position set to: (" << position.x << ", " << position.y << ")" << std::endl;
}

void TransformComponent::setPosition(const sf::Vector2f& position)
{
	this->position = position;
	//std::cout << "Position set to: (" << position.x << ", " << position.y << ")" << std::endl;
}

sf::Vector2f& TransformComponent::getPosition()
{
	return position;
}
