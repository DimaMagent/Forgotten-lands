#pragma once

#include <cstdint>
#include <vector>

namespace sl
{
	class Packet;

	//TODO возможно, стоит удалить этот класс, он пока ничего не особенно делает
	class PacketManager {
	public:
		PacketManager() = delete;
		static bool write(std::vector<uint8_t>& out, const Packet& packetData);
		static bool read(const std::vector<uint8_t>& in, Packet& outPacketData);
	};

}
