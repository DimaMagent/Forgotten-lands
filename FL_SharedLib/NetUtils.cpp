#include "pch.h"
#include "NetUtils.hpp"

void sl::net::write_uint8_t(std::vector<uint8_t>& out, uint8_t value) {
	 out.push_back(value);
}

void sl::net::write_uint16_t(std::vector<uint8_t>& out, uint16_t value)
{
	out.push_back((value >> 8) & 0xFF);
	out.push_back(value & 0xFF);
}

void sl::net::write_uint32_t(std::vector<uint8_t>& out, uint32_t value)
{
	out.push_back((value >> 24) & 0xFF);
	out.push_back((value >> 16) & 0xFF);
	out.push_back((value >> 8) & 0xFF);
	out.push_back(value & 0xFF);
}

uint8_t sl::net::read_uint8_t(const std::vector<uint8_t>& in, size_t& offset)
{
	if (offset + 1 <= in.size()) {
		return in[offset++];
	}
	return 0;
}

uint16_t sl::net::read_uint16_t(const std::vector<uint8_t>& in, size_t& offset)
{
	if (offset + 2 <= in.size()) {
		uint16_t value = (in[offset] << 8) | in[offset + 1];
		offset += 2;
		return value;
	}
	return 0;
}

uint32_t sl::net::read_uint32_t(const std::vector<uint8_t>& in, size_t& offset)
{
	if (offset + 4 <= in.size()) {
		uint32_t value = (in[offset] << 24) | (in[offset + 1] << 16) | (in[offset + 2] << 8) | in[offset + 3];
		offset += 4;
		return value;
	}
	return 0;
}