#include "pch.h"
#include "Serializable.hpp"
#include "NetUtils.hpp"

namespace sl {

	void Serializable::serializeVector2f(std::vector<uint8_t>& out, const sf::Vector2f& vec) const{
		net::write_uint32_t(out, vec.x);
		net::write_uint32_t(out, vec.y);
	}

	sf::Vector2f Serializable::deserializeVector2f(const std::vector<uint8_t>& out, size_t& offset) const {
		float x = net::read_uint32_t(out, offset);
		float y = net::read_uint32_t(out, offset);
		return sf::Vector2f(x, y);
	}
}