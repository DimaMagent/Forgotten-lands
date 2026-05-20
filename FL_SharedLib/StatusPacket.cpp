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
			write_uint8_t(out, d);
		}
		return true;
	}

	void StatusData::read(const std::vector<uint8_t>& in, size_t& offset)
	{
		if (offset < in.size()) {
			header.read(in, offset);
			if (in.size() > offset) {
				data.reserve(in.size());
				data.insert(data.end(), in.begin() + offset, in.end());
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
		size_t offset = 0;
		std::vector<EntityData> entitiesCompData;

		while (offset < data.size()) {
			EntityData enData;
			enData.entityId = sl::net::read_uint32_t(data, offset);
			enData.entityDataSize = sl::net::read_uint32_t(data, offset);
			size_t entityEnd = offset + enData.entityDataSize;

			while (offset < entityEnd) {
				ComponentData compData;
				compData.typeId = sl::net::read_uint32_t(data, offset);
				compData.compSize = sl::net::read_uint32_t(data, offset);
				compData.componentData.insert(compData.componentData.end(),
					data.begin() + offset,
					data.begin() + offset + compData.compSize);
				offset += compData.compSize;
				enData.componentsData.push_back(compData);
			}
			entitiesCompData.push_back(enData);
		}
		return entitiesCompData;
	}
}
