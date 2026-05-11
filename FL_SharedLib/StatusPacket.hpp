#pragma once
#include "Packet.hpp"

namespace sl::net {
	struct StatusData : public Data {
		std::vector<uint8_t> data;
		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		virtual void fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, std::vector<uint8_t> data);
	};
	class StatusPacket : public Packet {
	public:
		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		static constexpr PacketType type() { return PacketType::PT_Status; }
		virtual void fillPacketData(uint16_t sequenceNumber, uint32_t token, std::vector<uint8_t> data);
		const Header& getHeader() const override { return data.header; }
		const StatusData& getData() const override { return data; }
	private:
		StatusData data;
	};
}