#include "pch.hpp"
#include "OutputDataManager.hpp"
#include "Session.hpp"
#include "PacketManager.hpp"
#include "Packet.hpp"

OutputDataManager::OutputDataManager(std::unordered_map<uint32_t, std::weak_ptr<Session>>& sessions) :
	tokenToSessions(sessions)
{
}

void OutputDataManager::writePacket(const sl::net::Packet& packetData, uint32_t token)
{
	std::vector<uint8_t> outBuffer;
	if (sl::net::PacketManager::write(outBuffer, packetData)) {
		if (auto it = tokenToSessions.find(token); it != tokenToSessions.end())
			if (auto session = it->second.lock()) {
				session->writeOnOutgoingData(outBuffer);
			}
			else {
				std::cerr << "Failed to write packet data: session expired\n";
			}
		else {
			std::cerr << "Failed to write packet data: wrong token\n";
		}
	}
	else {
		std::cerr << "Failed to write packet data.\n";
	}
}