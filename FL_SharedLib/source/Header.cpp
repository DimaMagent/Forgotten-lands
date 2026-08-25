#include "pch.h"
#include "Header.hpp"

namespace sl::net {
	bool Header::write(std::vector<uint8_t>& out) const {
		net::write<uint32_t>(out, data.size);
		net::write<uint16_t>(out, data.sequenceNumber);
		net::write<std::underlying_type_t<PacketType>>(out, data.type);
		net::write<uint32_t>(out, data.token);
		return true;
	}

	void Header::read(const std::vector<uint8_t>& in, size_t& offset)
	{
		data.size = net::read<uint32_t>(in, offset);
		data.sequenceNumber = net::read<uint16_t>(in, offset);
		data.type = net::read<std::underlying_type_t<PacketType>>(in, offset);
		data.token = net::read<uint32_t>(in, offset);
	}

	void Header::fillHeader(uint16_t sequenceNumber, PacketType type, uint32_t token, uint32_t dataSize)
	{
		this->data.sequenceNumber = sequenceNumber;
		this->data.type = static_cast<std::underlying_type_t<PacketType>>(type);
		this->data.token = token;
		this->data.size = HEADER_SIZE + dataSize;
	}
}
