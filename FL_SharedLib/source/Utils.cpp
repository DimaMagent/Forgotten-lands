#include "pch.h"
#include <cmath>
#include "Utils.hpp"


sf::Vector2i sl::inBounds(const sf::Vector2i& vector, sf::Vector2i minBounds, sf::Vector2i maxBounds)
{
	sf::Vector2i boundedVector = vector;
	if (vector.x <= minBounds.x) {
		boundedVector.x = minBounds.x;
	}
	else if (vector.x >= maxBounds.x) {
		boundedVector.x = maxBounds.x;
	}

	if (vector.y <= minBounds.y) {
		boundedVector.y = minBounds.y;
	}
	else if (vector.y >= maxBounds.y) {
		boundedVector.y = maxBounds.y;
	}
	return boundedVector;
}

bool sl::areEqualAbsolute(float a, float b, float epsilon) {
	return std::abs(a - b) <= epsilon;
}
