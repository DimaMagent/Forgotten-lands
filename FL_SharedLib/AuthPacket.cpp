#include "pch.h"
#include "AuthPacket.hpp"

namespace sl::net {
    bool AuthData::write(std::vector<uint8_t>& out) const
    {
        header.write(out);
        net::write_uint32_t(out, playerEntityID);
        return true;
    }

    void AuthData::read(const std::vector<uint8_t>& in, size_t& offset)
    {
        if (offset < in.size()) {
            header.read(in, offset);
            playerEntityID = net::read_uint32_t(in, offset);
        }
    }

    void AuthData::fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t fromToken, uint32_t playerEntityID)
    {
        this->playerEntityID = playerEntityID;
        header.fillHeader(sequenceNumber, type, fromToken, sizeof(this->playerEntityID));
    }

    bool AuthPacket::write(std::vector<uint8_t>& out) const
    {
        return data.write(out);
    }

    void AuthPacket::read(const std::vector<uint8_t>& in, size_t& offset)
    {
        data.read(in, offset);
    }

    void AuthPacket::fillPacketData(uint16_t sequenceNumber, uint32_t FromToken, uint32_t playerEntityID)
    {
        data.fillPacketData(sequenceNumber, type(), FromToken, playerEntityID);
    }
}

