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
	bool areEqualAbsolute(float a, float b, float epsilon = 1e-5f);

	template<typename T>
	void swapPopDelete(std::vector<T>& vec, size_t popIndex) {
		vec[popIndex] = vec.back();
		vec.pop_back();
	}

	sf::Vector2f asNormalized(const sf::Vector2f& vector);
	sf::Vector2i asNormalized(const sf::Vector2i& vector);

	constexpr uint32_t fnv1a(const char* str) {
		uint32_t hash = 2166136261u;
		while (*str) {
			hash ^= (uint8_t)*str++;
			hash *= 16777619u;
		}
		return hash;
	}
}