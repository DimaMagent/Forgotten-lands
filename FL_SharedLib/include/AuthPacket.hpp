#pragma once
#include "Packet.hpp"
#include "StatusPacket.hpp" //temp

/*TODO: Данный пакет нужно переделать в пакет для авторизации*/
namespace sl::net {
	struct AuthData : Data {
		uint32_t playerEntityID;
		std::vector<uint8_t> statData;
		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		virtual void fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t fromToken, uint32_t playerEntityID, std::vector<uint8_t> statusData);
		std::vector<sl::net::EntityData> getEntityData() const; // temp
	};

	class AuthPacket : public Packet {
	public:
		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		static constexpr PacketType type() { return PacketType::PT_Auth; }
		virtual void fillPacketData(uint16_t sequenceNumber, uint32_t fromToken, uint32_t playerEntityID, std::vector<uint8_t> statusData);
		const Header& getHeader() const override { return data.header; }
		const AuthData& getData() const override { return data; }
	private:
		AuthData data;
	};
}