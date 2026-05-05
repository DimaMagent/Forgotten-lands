#include "pch.hpp"
#include "OutputDataManager.hpp"
#include "ClientSession.hpp"
#include "PacketManager.hpp"
#include "Packet.hpp"

OutputDataManager::OutputDataManager(std::weak_ptr<ClientSession> session):
	session(session)
{
}

void OutputDataManager::writePacket(const sl::net::Packet& packetData)
{
	std::vector<uint8_t> outBuffer;
	if (sl::net::PacketManager::write(outBuffer, packetData)) {
		if (auto sessionPtr = session.lock()) {
			sessionPtr->writeOnOutgoingData(outBuffer);
		} else {
			std::cerr << "Failed to write packet data: session is no longer available." << "\n";
		}
	}
	else {
		std::cerr << "Failed to write packet data." << "\n";
	}
}