#include "pch.h"
#include "Header.hpp"

bool sl::Header::write(std::vector<uint8_t>& out) const {
	write_uint32_t(out, size);
	write_uint16_t(out, sequenceNumber);
	write_uint8_t(out, type);
	return true;
}

void sl::Header::read(const std::vector<uint8_t>& in, size_t& offset)
{
	size = read_uint32_t(in, offset);
	sequenceNumber = read_uint16_t(in, offset);
	type = read_uint8_t(in, offset);
}

void sl::Header::fillHeader(uint16_t sequenceNumber, PacketType type, uint32_t dataSize)
{
	this->sequenceNumber = sequenceNumber;
	this->type = static_cast<uint8_t>(type);
	this->size = sizeof(sequenceNumber) + sizeof(type) + dataSize;
}
