#include "pch.hpp"
#include "OutputDataManager.hpp"
#include "Session.hpp"
#include "PacketManager.hpp"
#include "Packet.hpp"

OutputDataManager::OutputDataManager(std::unordered_map<uint32_t, std::weak_ptr<Session>>& sessions) :
	tokenToSessions(sessions)
{
	logger = spdlog::get("network");
}

void OutputDataManager::writePacket(const sl::net::Packet& packetData, uint32_t token)
{
	std::vector<uint8_t> outBuffer;
	if (sl::net::PacketManager::write(outBuffer, packetData)) {
		if (auto it = tokenToSessions.find(token); it != tokenToSessions.end()) {
			if (auto session = it->second.lock()) {
				session->writeOnOutgoingData(outBuffer);
			}
			else {
				logger->warn("Failed to write packet data: session expired for token {}", token);
			}
		}
		else {
			logger->warn("Failed to write packet data: no session found for token {}", token);
		}
	}
	else {
		logger->warn("Failed to write packet data: serialization failed for token {}", token);
	}
}