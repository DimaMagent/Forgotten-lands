#include "pch.h"
#include "PacketData.hpp"

void sl::PacketData::write_uint8_t(std::vector<uint8_t>& out, uint8_t value) const {
	 out.push_back(value);
}

void sl::PacketData::write_uint16_t(std::vector<uint8_t>& out, uint16_t value) const
{
	out.push_back((value >> 8) & 0xFF);
	out.push_back(value & 0xFF);
}

void sl::PacketData::write_uint32_t(std::vector<uint8_t>& out, uint32_t value) const
{
	out.push_back((value >> 24) & 0xFF);
	out.push_back((value >> 16) & 0xFF);
	out.push_back((value >> 8) & 0xFF);
	out.push_back(value & 0xFF);
}

uint8_t sl::PacketData::read_uint8_t(const std::vector<uint8_t>& in, size_t& offset)
{
	if (offset + 1 <= in.size()) {
		return in[offset];
		offset += 1;
	}
	return 0;
}

uint16_t sl::PacketData::read_uint16_t(const std::vector<uint8_t>& in, size_t& offset)
{
	if (offset + 2 <= in.size()) {
		uint16_t value = (in[offset] << 8) | in[offset + 1];
		offset += 2;
		return value;
	}
	return 0;
}

uint32_t sl::PacketData::read_uint32_t(const std::vector<uint8_t>& in, size_t& offset)
{
	if (offset + 4 <= in.size()) {
		uint32_t value = (in[offset] << 24) | (in[offset + 1] << 16) | (in[offset + 2] << 8) | in[offset + 3];
		offset += 4;
		return value;
	}
	return 0;
}
