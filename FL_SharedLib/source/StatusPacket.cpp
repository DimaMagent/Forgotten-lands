#include "pch.h"
#include "StatusPacket.hpp"

namespace sl::net {
	bool StatusPacket::write(std::vector<uint8_t>& out) const
	{
		return data.write(out);
	}

	void StatusPacket::read(const std::vector<uint8_t>& in, size_t& offset)
	{
		data.read(in, offset);
	}

	void StatusPacket::readData(const sl::net::StatusData& statData)
	{
		this->data.readData(statData);
	}

	void StatusPacket::fillPacketData(uint16_t sequenceNumber, uint32_t token, std::vector<uint8_t> data)
	{
		this->data.fillPacketData(sequenceNumber, type(), token, data);
	}

	bool StatusData::write(std::vector<uint8_t>& out) const
	{
		header.write(out);
		for (uint8_t d : data) {
			sl::net::write<uint8_t>(out, d);
		}
		return true;
	}

	void StatusData::read(const std::vector<uint8_t>& in, size_t& offset)
	{
		if (offset + sl::net::Header::HEADER_SIZE <= in.size()) {
			header.read(in, offset);
			if (offset + data.size() < in.size()) {
				data.reserve(in.size());
				data.insert(data.end(), in.begin() + offset, in.end());
				offset += data.size();
			}
		}
	}

	void StatusData::readData(const sl::net::StatusData& statData)
	{
		this->data = statData.data;
	}

	void StatusData::fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, std::vector<uint8_t> data)
	{
		this->data = data;
		header.fillHeader(sequenceNumber, type, token, data.size() * sizeof(decltype(data)::value_type));
	}

	std::vector<EntityData> StatusData::getEntityData() const
	{
		return sl::net::EntityData::getEntityData(data);
	}
}
