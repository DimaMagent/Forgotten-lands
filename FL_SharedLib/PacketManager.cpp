#include "pch.h"
#include "PacketManager.hpp"
#include "Packet.hpp"

namespace sl::net {
	bool PacketManager::write(std::vector<uint8_t>& out, const Packet& packetData)
	{
		return packetData.write(out);
	}


	bool PacketManager::read(const std::vector<uint8_t>& in, Packet& outPacketData)
	{
		size_t offset = 0;
		outPacketData.read(in, offset);
		return true;
	}
}
