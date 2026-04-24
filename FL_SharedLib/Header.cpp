#include "pch.h"
#include "Header.hpp"

bool sl::Header::write(std::vector<uint8_t>& out) const {
	net::write_uint32_t(out, data.size);
	net::write_uint16_t(out, data.sequenceNumber);
	net::write_uint8_t(out, data.type);
	return true;
}

void sl::Header::read(const std::vector<uint8_t>& in, size_t& offset)
{
	data.size = net::read_uint32_t(in, offset);
	data.sequenceNumber = net::read_uint16_t(in, offset);
	data.type = net::read_uint8_t(in, offset);
}

void sl::Header::fillHeader(uint16_t sequenceNumber, PacketType type, uint32_t dataSize)
{
	this->data.sequenceNumber = sequenceNumber;
	this->data.type = static_cast<uint8_t>(type);
	this->data.size = sizeof(sequenceNumber) + sizeof(type) + dataSize;
}
