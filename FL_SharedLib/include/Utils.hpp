#pragma once
#include "SFML/System/Vector2.hpp"

namespace sl {
	sf::Vector2i inBounds(const sf::Vector2i& vector, sf::Vector2i minBounds, sf::Vector2i maxBounds);
}