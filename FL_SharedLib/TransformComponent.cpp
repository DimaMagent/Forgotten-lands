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

void sl::TransformComponent::loadFromJSON(const nlohmann::json& data)
{
	auto position = data.find("position");
	if (position != data.end()) {
		setPosition(sf::Vector2f(position->value("x", 0.f), position->value("y", 0.f)));
	}
}

void sl::TransformComponent::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	//std::cout << "Position set to: (" << position.x << ", " << position.y << ")" << std::endl;
}

void sl::TransformComponent::setPosition(const sf::Vector2f& position)
{
	this->position = position;
	//std::cout << "Position set to: (" << position.x << ", " << position.y << ")" << std::endl;
}

sf::Vector2f& sl::TransformComponent::getPosition()
{
	return position;
}
