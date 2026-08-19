#include "pch.h"
#include "Packet.hpp"

namespace sl::net {
	std::optional<Header> Packet::readHeader(const std::vector<uint8_t>& in, size_t& offset)
	{
		Header header;
		HeaderData headerData = header.getData();
		if (in.size() < sl::net::Header::HEADER_SIZE) {
			return std::nullopt;
		}
		header.read(in, offset);
		return header;
	}
}