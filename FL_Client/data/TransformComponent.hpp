#pragma once
#include <SFML/System/Vector2.hpp>

class TransformComponent {
public:
	TransformComponent();
	TransformComponent(sf::Vector2f startPosition);
	TransformComponent(float x, float y);
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f& getPosition();
private:
	sf::Vector2f position;
};