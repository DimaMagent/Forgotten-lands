#pragma once
#include <cstdint>
#include <vector>
#include "Packet.hpp"

namespace sl{

	struct InputStateData : public Data {
		uint8_t inputState;
		bool pressingFlag;
		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		virtual void fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, uint8_t inputState, bool pressingFlag);
	};

	class InputStatePacket: public Packet {
		public:
			virtual bool write(std::vector<uint8_t>& out) const override;
			virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
			virtual void fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, uint8_t inputState, bool pressingFlag);
			const Header& getHeader() const override { return data.header; }
			const InputStateData& getData() const override { return data; }
		private:
			InputStateData data;
	};

}
