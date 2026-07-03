#pragma once
#include "Packet.hpp"
#include <vector>

namespace sl::net {
	struct ComponentData {
		uint32_t typeId = 0;
		uint32_t compSize = 0;
		std::vector<uint8_t> componentData;
	};

	struct EntityData {
		uint32_t entityId = 0;
		uint32_t entityDataSize = 0;
		std::vector<ComponentData> componentsData;
	};

	struct StatusData : public Data {
		std::vector<uint8_t> data;
		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		void readData(const sl::net::StatusData& statData);
		virtual void fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, std::vector<uint8_t> data);
		std::vector<EntityData> getEntityData() const;
	};
	class StatusPacket : public Packet {
	public:
		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		void readData(const sl::net::StatusData& statData);
		static constexpr PacketType type() { return PacketType::PT_Status; }
		virtual void fillPacketData(uint16_t sequenceNumber, uint32_t token, std::vector<uint8_t> data);
		const Header& getHeader() const override { return data.header; }
		const StatusData& getData() const override { return data; }
	private:
		StatusData data;
	};
}