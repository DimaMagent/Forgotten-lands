#pragma once
#include <cstdint>
#include <vector>

namespace sl::net
{
	void write_uint8_t(std::vector<uint8_t>& out, uint8_t value);
	void write_uint16_t(std::vector<uint8_t>& out, uint16_t value);
	void write_uint32_t(std::vector<uint8_t>& out, uint32_t value);
	uint8_t read_uint8_t(const std::vector<uint8_t>& in, size_t& offset);
	uint16_t read_uint16_t(const std::vector<uint8_t>& in, size_t& offset);
	uint32_t read_uint32_t(const std::vector<uint8_t>& in, size_t& offset);
}