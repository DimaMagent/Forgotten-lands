#pragma once
#include <cstdint>
#include <vector>
#include "PacketData.hpp"

namespace sl
{
	enum PacketType : uint8_t {
		PT_Unknown = 0,
		PT_InputState = 1,
		PT_Input = 2,
		PT_PlayerPosition = 3,
		PT_GameStateUpdate = 4,
	};

	struct Header : public PacketData {
	public:
		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		void fillHeader(uint16_t sequenceNumber, PacketType type, uint32_t dataSize);
	private:
		uint32_t size = 0;
		uint16_t sequenceNumber = 0;
		uint8_t type = 0;
	};
}