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
	if (auto iq = incQueue.lock()) {
		iq->onDataPushed.addFunction([this]() { onDataPushed(); });
	}
}

void IncomingDataManager::assemblePacket()
{
	while (buffer.size() >= WIRE_HEADER_SIZE) {
		size_t offset = 0;

		uint32_t headerSize = sl::net::read_uint32_t(buffer, offset);

		if (headerSize == 0 || headerSize > MAX_PACKET_PAYLOAD) {
			std::cerr << "Invalid packet size: " << headerSize << ", dropping client\n";
			buffer.clear();
			OnWrongData.broadcast();
			return;
		}

		size_t totalPacketBytes = sizeof(uint32_t) + static_cast<size_t>(headerSize);

		if (buffer.size() < totalPacketBytes) {
			return;
		}
		uint16_t sequenceNumber = sl::net::read_uint16_t(buffer, offset);

		sl::net::PacketType ptype = static_cast<sl::net::PacketType>(sl::net::read_uint8_t(buffer, offset));

		uint32_t token = sl::net::read_uint32_t(buffer, offset);

		std::vector<uint8_t> packetBytes(buffer.begin(), buffer.begin() + totalPacketBytes);

		dataProcessorManager.routeData(packetBytes, ptype, token);

		buffer.erase(buffer.begin(), buffer.begin() + totalPacketBytes);

	}
}

void IncomingDataManager::onDataPushed()
{
	if (auto queue = incomingQueue.lock()) {
		std::vector<uint8_t> chunk;
		while (queue->tryPop(chunk)) {
			if (buffer.size() + chunk.size() > MAX_BUFFER_SIZE) {
				std::cerr << "Buffer overflow from client, dropping connection\n";
				buffer.clear();
				OnWrongData.broadcast();
				return;
			}
			buffer.insert(buffer.end(), chunk.begin(), chunk.end());
			assemblePacket();
		}
	}
	else {
		std::cerr << "Failed to process incoming data: DataQueue is no longer available." << "\n";
	}
}
