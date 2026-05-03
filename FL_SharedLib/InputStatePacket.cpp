#include "pch.h"
#include "InputStatePacket.hpp"

bool sl::InputStatePacket::write(std::vector<uint8_t>& out) const
{
	return data.write(out);
}

void sl::InputStatePacket::read(const std::vector<uint8_t>& in, size_t& offset)
{
	data.read(in, offset);
}


void sl::InputStatePacket::fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, uint8_t inputState, bool pressingFlag)
{
	data.fillPacketData(sequenceNumber, type, token, inputState, pressingFlag);
}

bool sl::InputStateData::write(std::vector<uint8_t>& out) const
{
	header.write(out);
	net::write_uint8_t(out, inputState);
	net::write_uint8_t(out, pressingFlag);
	return true;
}

void sl::InputStateData::read(const std::vector<uint8_t>& in, size_t& offset)
{
	if (offset < in.size()) {
		header.read(in, offset);
		inputState = in[offset++];
		pressingFlag = in[offset++];

	}
}

void sl::InputStateData::fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, uint8_t inputState, bool pressingFlag)
{
	this->inputState = inputState;
	this->pressingFlag = pressingFlag;
	header.fillHeader(sequenceNumber, type, token, sizeof(inputState) + sizeof(pressingFlag));
}
