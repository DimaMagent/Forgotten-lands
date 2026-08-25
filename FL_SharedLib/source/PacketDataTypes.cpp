#include "pch.h"
#include "PacketDataTypes.hpp"
#include "NetUtils.hpp"

std::vector<sl::net::EntityData> sl::net::EntityData::getEntityData(const std::vector<uint8_t>& data)
{
	size_t offset = 0;
	std::vector<sl::net::EntityData> entitiesCompData;

	while (offset < data.size()) {
		sl::net::EntityData enData;
		enData.entityId = sl::net::read<uint32_t>(data, offset);
		enData.entityType = sl::net::read<uint32_t>(data, offset);
		enData.entityDataSize = sl::net::read<uint32_t>(data, offset);
		size_t entityEnd = offset + enData.entityDataSize;

		while (offset < entityEnd) {
			sl::net::ComponentData compData;
			compData.typeId = sl::net::read<uint32_t>(data, offset);
			compData.compSize = sl::net::read<uint32_t>(data, offset);
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

