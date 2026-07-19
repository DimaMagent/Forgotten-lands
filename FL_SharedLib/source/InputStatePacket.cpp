#include "pch.h"
#include "InputStatePacket.hpp"

namespace sl::net {
	bool InputStatePacket::write(std::vector<uint8_t>& out) const
	{
		return data.write(out);
	}

	void InputStatePacket::read(const std::vector<uint8_t>& in, size_t& offset)
	{
		data.read(in, offset);
	}


	void InputStatePacket::fillPacketData(uint16_t sequenceNumber, uint32_t token, sf::Vector2i movementDirectionIntentions, uint8_t inputState, uint8_t inputAction)
	{
		data.fillPacketData(sequenceNumber, type(), token, movementDirectionIntentions, inputState, inputAction);
	}

	bool InputStateData::write(std::vector<uint8_t>& out) const
	{
		header.write(out);
		net::write_uint32_t(out, movementDirectionIntentions.x);
		net::write_uint32_t(out, movementDirectionIntentions.y);
		net::write_uint8_t(out, inputState);
		net::write_uint8_t(out, inputAction);
		return true;
	}

	void InputStateData::read(const std::vector<uint8_t>& in, size_t& offset)
	{
		if (offset < in.size()) {
			header.read(in, offset);
			movementDirectionIntentions.x = net::read_uint32_t(in, offset);
			movementDirectionIntentions.y = net::read_uint32_t(in, offset);
			inputState = net::read_uint8_t(in, offset);
			inputAction = net::read_uint8_t(in, offset);
		}
	}

	void InputStateData::fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, sf::Vector2i movementDirectionIntentions, uint8_t inputState, uint8_t inputAction)
	{
		this->movementDirectionIntentions = movementDirectionIntentions;
		this->inputState = inputState;
		this->inputAction = inputAction;
		header.fillHeader(sequenceNumber, type, token, sizeof(inputState) + sizeof(movementDirectionIntentions) + sizeof(inputAction));
	}
}