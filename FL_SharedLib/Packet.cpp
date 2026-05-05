#include "pch.h"
#include "Packet.hpp"

namespace sl::net {
	std::optional<Header> Packet::readHeader(const std::vector<uint8_t>& in, size_t& offset)
	{
		Header header;
		HeaderData headerData = header.getData();
		if (in.size() < sizeof(headerData.sequenceNumber) + sizeof(headerData.type) + sizeof(headerData.token)) {
			return std::nullopt;
		}
		header.read(in, offset);
		return header;
	}
}