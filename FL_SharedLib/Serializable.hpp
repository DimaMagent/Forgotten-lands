#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

namespace sl {
	class Serializable {
	public:
		//serialize functions return the size of the serialized data
		virtual void serialize(std::vector<uint8_t>& out) const = 0;
		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) = 0;
		virtual uint32_t getSerializeDataSize() const = 0;
	protected:
		void serializeVector2f(std::vector<uint8_t>& out, const sf::Vector2f& vec) const;
		sf::Vector2f deserializeVector2f(const std::vector<uint8_t>& out, size_t& offset) const;
	};
}