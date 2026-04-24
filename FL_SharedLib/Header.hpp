#pragma once
#include <cstdint>
#include <vector>
#include "PacketData.hpp"
#include "PacketDataTypes.hpp"

namespace sl
{


	struct HeaderData {
		uint32_t size = 0;
		uint16_t sequenceNumber = 0;
		uint8_t type = 0;
	};

	struct Header : public PacketData {
	public:
		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		void fillHeader(uint16_t sequenceNumber, PacketType type, uint32_t dataSize);
		HeaderData getData() const { return data; }
	private:
		HeaderData data;
	};
}