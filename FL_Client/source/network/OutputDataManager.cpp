#include "pch.hpp"
#include "OutputDataManager.hpp"
#include "ClientSession.hpp"
#include "PacketManager.hpp"
#include "Packet.hpp"

OutputDataManager::OutputDataManager(std::weak_ptr<ClientSession> session):
	session(session)
{
	logger = spdlog::get("network");
}

void OutputDataManager::writePacket(const sl::net::Packet& packetData)
{
	std::vector<uint8_t> outBuffer;
	if (sl::net::PacketManager::write(outBuffer, packetData)) {
		if (auto sessionPtr = session.lock()) {
			sessionPtr->writeOnOutgoingData(outBuffer);
		} else {
			logger->error("Failed to write packet data: session is no longer available.");
		}
	}
	else {
		logger->warn("Failed to write packet data.");
	}
}