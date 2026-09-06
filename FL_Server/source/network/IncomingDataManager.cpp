#include "pch.hpp"
#include "IncomingDataManager.hpp"
#include "DataQueue.hpp"
#include "PacketManager.hpp"
#include "Packet.hpp"
#include "Header.hpp"
#include "DataProcessorManager.hpp"
#include "NetUtils.hpp"

namespace {
	constexpr size_t WIRE_HEADER_SIZE = sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint8_t) + sizeof(uint32_t);
	constexpr size_t MAX_PACKET_PAYLOAD = 65535;
	constexpr size_t MAX_BUFFER_SIZE = 256 * 1024;
}

IncomingDataManager::IncomingDataManager(std::weak_ptr<sl::net::DataQueue> incQueue, DataProcessorManager& dpm, uint32_t token) :
	incomingQueue(incQueue), dataProcessorManager(dpm), token(token)
{
	logger = spdlog::get("network");
	if (auto iq = incQueue.lock()) {
		iq->onDataPushed.addFunction([this]() { onDataPushed(); });
	}
}

void IncomingDataManager::assemblePacket()
{
	while (buffer.size() >= WIRE_HEADER_SIZE) {
		size_t offset = 0;

		sl::net::Header header;

		header.read(buffer, offset);

		uint32_t headerSize = header.getData().size;

		if (headerSize == 0 || headerSize > MAX_PACKET_PAYLOAD) {
			logger->warn("Invalid packet size: {}, dropping client {}", headerSize, token);
			buffer.clear();
			OnWrongData.broadcast();
			return;
		}

		size_t totalPacketBytes = static_cast<size_t>(headerSize);

		if (buffer.size() < totalPacketBytes) {
			return;
		}

		sl::net::PacketType ptype = static_cast<sl::net::PacketType>(header.getData().type);

		std::vector<uint8_t> packetBytes(buffer.begin(), buffer.begin() + totalPacketBytes);

		dataProcessorManager.routeData(std::move(packetBytes), ptype, this->token);

		buffer.erase(buffer.begin(), buffer.begin() + totalPacketBytes);
	}
}

void IncomingDataManager::onDataPushed()
{
	if (auto queue = incomingQueue.lock()) {
		std::vector<uint8_t> chunk;
		while (queue->tryPop(chunk)) {
			if (buffer.size() + chunk.size() > MAX_BUFFER_SIZE) {
				logger->warn("Buffer overflow from client {}, dropping connection", token);
				buffer.clear();
				OnWrongData.broadcast();
				return;
			}
			buffer.insert(buffer.end(), chunk.begin(), chunk.end());
			assemblePacket();
		}
	}
	else {
		logger->error("Failed to process incoming data: DataQueue is no longer available for client {}", token);
	}
}
