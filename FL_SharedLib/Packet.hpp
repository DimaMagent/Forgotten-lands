#pragma once
#include "PacketData.hpp"
#include "Header.hpp"
#include <vector>
#include <cstdint>
#include <optional>

namespace sl
{
	struct Data: public PacketData {
		Header header;
		virtual void fillPacketData(uint16_t sequenceNumber, PacketType type, uint8_t inputState) = 0;
	};


	class Packet {
	public:		
		virtual ~Packet() = default;
		virtual bool write(std::vector<uint8_t>& out) const = 0;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) = 0;
		static std::optional<Header> readHeader(const std::vector<uint8_t>& in, size_t& offset);
		virtual void fillPacketData(uint16_t sequenceNumber, PacketType type, uint8_t inputState) = 0;
		virtual const Header& getHeader() const = 0;
		virtual const Data& getData() const = 0;
	};

}