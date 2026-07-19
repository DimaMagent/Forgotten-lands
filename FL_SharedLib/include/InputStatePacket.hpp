#pragma once
#include <cstdint>
#include <vector>
#include "Packet.hpp"
#include "SFML/System/Vector2.hpp"

namespace sl::net {

	struct InputStateData : public Data {
		sf::Vector2i movementDirectionIntentions;
		uint8_t inputState;
		uint8_t inputAction;

		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		virtual void fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, sf::Vector2i movementDirectionIntentions, uint8_t inputState, uint8_t inputAction);
	};

	class InputStatePacket: public Packet {
		public:
			virtual bool write(std::vector<uint8_t>& out) const override;
			virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
			static constexpr PacketType type() { return PacketType::PT_InputState; }
			virtual void fillPacketData(uint16_t sequenceNumber, uint32_t token, sf::Vector2i movementDirectionIntentions, uint8_t inputState, uint8_t inputAction);
			const Header& getHeader() const override { return data.header; }
			const InputStateData& getData() const override { return data; }
		private:
			InputStateData data;
	};

}
