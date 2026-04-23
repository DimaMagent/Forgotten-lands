#include "pch.h"
#include "Packet.hpp"

std::optional<sl::Header> sl::Packet::readHeader(const std::vector<uint8_t>& in, size_t& offset)
{
	sl::Header header;
	if (in.size() < sizeof(sl::Header)) {
		return std::nullopt;
	}
	header.read(in, offset);
	return header;
}