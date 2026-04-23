#include "pch.h"
#include "InputStatePacket.hpp"

bool sl::InputStatePacket::write(std::vector<uint8_t>& out) const
{
	return data.write(out);
}

void sl::InputStatePacket::read(const std::vector<uint8_t>& in, size_t& offset)
{
	return data.read(in, offset);
}


void sl::InputStatePacket::fillPacketData(uint16_t sequenceNumber, PacketType type, uint8_t inputState)
{
	data.fillPacketData(sequenceNumber, type, inputState);
}

bool sl::InputStateData::write(std::vector<uint8_t>& out) const
{
	header.write(out);
	write_uint8_t(out, inputState);
	return true;
}

void sl::InputStateData::read(const std::vector<uint8_t>& in, size_t& offset)
{
	if (offset < in.size()) {
		header.read(in, offset);
		inputState = in[offset];
		offset += 1;
	}
}

void sl::InputStateData::fillPacketData(uint16_t sequenceNumber, PacketType type, uint8_t inputState)
{
	this->inputState = inputState;
	header.fillHeader(sequenceNumber, type, sizeof(inputState));
}
