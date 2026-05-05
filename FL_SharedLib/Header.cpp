#include "pch.h"
#include "Header.hpp"

namespace sl::net {
	bool Header::write(std::vector<uint8_t>& out) const {
		net::write_uint32_t(out, data.size);
		net::write_uint16_t(out, data.sequenceNumber);
		net::write_uint8_t(out, data.type);
		net::write_uint32_t(out, data.token);
		return true;
	}

	void Header::read(const std::vector<uint8_t>& in, size_t& offset)
	{
		data.size = net::read_uint32_t(in, offset);
		data.sequenceNumber = net::read_uint16_t(in, offset);
		data.type = net::read_uint8_t(in, offset);
		data.token = net::read_uint32_t(in, offset);
	}

	void Header::fillHeader(uint16_t sequenceNumber, PacketType type, uint32_t token, uint32_t dataSize)
	{
		this->data.sequenceNumber = sequenceNumber;
		this->data.type = static_cast<uint8_t>(type);
		this->data.token = token;
		this->data.size = sizeof(sequenceNumber) + sizeof(type) + sizeof(token) + dataSize;
	}
}
