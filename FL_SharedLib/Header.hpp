#pragma once
#include <cstdint>
#include <vector>
#include "NetUtils.hpp"
#include "PacketDataTypes.hpp"

namespace sl::net {
	struct HeaderData {
		uint32_t size = 0;
		uint16_t sequenceNumber = 0;
		uint8_t type = 0;
		uint32_t token = 0;
	};

	struct Header {
	public:
		virtual bool write(std::vector<uint8_t>& out) const;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset);
		void fillHeader(uint16_t sequenceNumber, PacketType type, uint32_t token, uint32_t dataSize);
		HeaderData getData() const { return data; }
	private:
		HeaderData data;
	};
}