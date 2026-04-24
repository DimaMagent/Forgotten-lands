#include "pch.hpp"
#include "IncomingDataManager.hpp"
#include "DataQueue.hpp"
#include "PacketManager.hpp"
#include "Packet.hpp"
#include "Header.hpp"
#include "DataProcessorManager.hpp"
#include "NetUtils.hpp"

IncomingDataManager::IncomingDataManager(std::shared_ptr<sl::DataQueue> incQueue, DataProcessorManager& dpm) : incomingQueue(incQueue), dataProcessorManager(dpm)
{
	incQueue->onDataPushed.addFunction([this]() {
		if (auto queue = incomingQueue.lock()) {
			std::vector<uint8_t> chunk;
			while (queue->tryPop(chunk)) {
				buffer.insert(buffer.end(), chunk.begin(), chunk.end());
				assemblePacket();
			}
		}
		else {
			std::cerr << "Failed to process incoming data: DataQueue is no longer available." << "\n";
		}
	});
}

namespace {
	constexpr size_t WIRE_HEADER_SIZE = sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint8_t);
}

void IncomingDataManager::assemblePacket()
{
	while (buffer.size() >= WIRE_HEADER_SIZE) {
		size_t offset = 0;

		uint32_t headerSize = sl::net::read_uint32_t(buffer, offset);
		size_t totalPacketBytes = sizeof(uint32_t) + static_cast<size_t>(headerSize);

		if (buffer.size() < totalPacketBytes) {
			return;
		}
		uint16_t sequenceNumber = sl::net::read_uint16_t(buffer, offset);

		sl::PacketType ptype = static_cast<sl::PacketType>(sl::net::read_uint8_t(buffer, offset));

		std::vector<uint8_t> packetBytes(buffer.begin(), buffer.begin() + totalPacketBytes);

		dataProcessorManager.routeData(packetBytes, ptype);

		buffer.erase(buffer.begin(), buffer.begin() + totalPacketBytes);

	}
}
