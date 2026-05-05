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


	void InputStatePacket::fillPacketData(uint16_t sequenceNumber, uint32_t token, uint8_t inputState, bool pressingFlag)
	{
		data.fillPacketData(sequenceNumber, type(), token, inputState, pressingFlag);
	}

	bool InputStateData::write(std::vector<uint8_t>& out) const
	{
		header.write(out);
		net::write_uint8_t(out, inputState);
		net::write_uint8_t(out, pressingFlag);
		return true;
	}

	void InputStateData::read(const std::vector<uint8_t>& in, size_t& offset)
	{
		if (offset < in.size()) {
			header.read(in, offset);
			inputState = net::read_uint8_t(in, offset);
			pressingFlag = net::read_uint8_t(in, offset);

		}
	}

	void InputStateData::fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, uint8_t inputState, bool pressingFlag)
	{
		this->inputState = inputState;
		this->pressingFlag = pressingFlag;
		header.fillHeader(sequenceNumber, type, token, sizeof(inputState) + sizeof(pressingFlag));
	}
}