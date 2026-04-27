#pragma once
#include <SFML/System/Vector2.hpp>
#include "Component.hpp"

namespace sl {

	/*Component that stores position of an entity. Can be extended in the future to include rotation, scale, etc.*/
	class TransformComponent: public Component {
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
}