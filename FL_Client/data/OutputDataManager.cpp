#include "pch.hpp"
#include "OutputDataManager.hpp"
#include "ClientSession.hpp"
#include "PacketManager.hpp"
#include "Packet.hpp"

OutputDataManager::OutputDataManager(std::shared_ptr<ClientSession> session):
	session(std::move(session))
{
}

void OutputDataManager::writePacket(const sl::Packet& packetData)
{
	std::vector<uint8_t> outBuffer;
	if (sl::PacketManager::write(outBuffer, packetData)) {
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