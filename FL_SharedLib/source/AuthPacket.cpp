#include "pch.h"
#include "AuthPacket.hpp"

namespace sl::net {
    bool AuthData::write(std::vector<uint8_t>& out) const
    {
        header.write(out);
        net::write_uint32_t(out, playerEntityID);
        for (uint8_t d : statData) {
            net::write_uint8_t(out, d);
        }
        return true;
    }

    void AuthData::read(const std::vector<uint8_t>& in, size_t& offset)
    {
        if (offset < in.size()) {
            header.read(in, offset);
            playerEntityID = net::read_uint32_t(in, offset);
            if (in.size() > offset) {
                statData.reserve(in.size());
                statData.insert(statData.end(), in.begin() + offset, in.end());
                offset += statData.size();
            }
        }
    }

    void AuthData::fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t fromToken, uint32_t playerEntityID, std::vector<uint8_t> statusData)
    {
        this->playerEntityID = playerEntityID;
        this->statData = statusData;
        header.fillHeader(sequenceNumber, type, fromToken, sizeof(this->playerEntityID) + statData.size() * sizeof(decltype(statData)::value_type));
    }

    std::vector<sl::net::EntityData> AuthData::getEntityData() const {
        size_t offset = 0;
        std::vector<EntityData> entitiesCompData;

        while (offset < statData.size()) {
            EntityData enData;
            enData.entityId = sl::net::read_uint32_t(statData, offset);
            enData.entityType = sl::net::read_uint32_t(statData, offset);
            enData.entityDataSize = sl::net::read_uint32_t(statData, offset);
            size_t entityEnd = offset + enData.entityDataSize;

            while (offset < entityEnd) {
                ComponentData compData;
                compData.typeId = sl::net::read_uint32_t(statData, offset);
                compData.compSize = sl::net::read_uint32_t(statData, offset);
                compData.componentData.insert(compData.componentData.end(),
                    statData.begin() + offset,
                    statData.begin() + offset + compData.compSize);
                offset += compData.compSize;
                enData.componentsData.push_back(compData);
            }
            entitiesCompData.push_back(enData);
        }
        return entitiesCompData;
    }

    bool AuthPacket::write(std::vector<uint8_t>& out) const
    {
        return data.write(out);
    }

    void AuthPacket::read(const std::vector<uint8_t>& in, size_t& offset)
    {
        data.read(in, offset);
    }

    void AuthPacket::fillPacketData(uint16_t sequenceNumber, uint32_t FromToken, uint32_t playerEntityID, std::vector<uint8_t> statusData)
    {
        data.fillPacketData(sequenceNumber, type(), FromToken, playerEntityID, statusData);
    }
}

