#pragma once
#include "SFML/System/Vector2.hpp"

namespace sl {
	sf::Vector2i inBounds(const sf::Vector2i& vector, sf::Vector2i minBounds, sf::Vector2i maxBounds);

	template<typename T>
	T boundNumber(const T& num, T min, T max) {
		if (num >= max) {
			return max;
		}
		else if (num <= min) {
			return min;
		}
		return num;
	}
}