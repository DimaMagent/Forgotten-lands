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

sf::Vector2f sl::asNormalized(const sf::Vector2f& vector)
{
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length > 0.0001f) {
		return sf::Vector2f(vector.x / length, vector.y / length);
	}
	else {
		return sf::Vector2f(0.f, 0.f);
	}
}

sf::Vector2i sl::asNormalized(const sf::Vector2i& vector)
{
	double length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length >= 0.0001) {
		return sf::Vector2i(std::round(vector.x / length), std::round(vector.y / length));
	}
	else {
		return sf::Vector2i(0, 0);
	}
}
