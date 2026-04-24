#include "pch.hpp"
#include "IncomingDataManager.hpp"
#include "DataQueue.hpp"
#include "PacketManager.hpp"
#include "Packet.hpp"
#include "Header.hpp"
#include "DataProcessorManager.hpp"

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

	static uint32_t readLE32(const std::vector<uint8_t>& b, size_t off) {
		return uint32_t(b[off]) | (uint32_t(b[off + 1]) << 8) | (uint32_t(b[off + 2]) << 16) | (uint32_t(b[off + 3]) << 24);
	}
	static uint16_t readLE16(const std::vector<uint8_t>& b, size_t off) {
		return uint16_t(b[off]) | (uint16_t(b[off + 1]) << 8);
	}
	static uint8_t read8(const std::vector<uint8_t>& b, size_t off) {
		return b[off];
	}
}

void IncomingDataManager::assemblePacket()
{
	size_t offset = 0;
	while (buffer.size() >= WIRE_HEADER_SIZE) {
		uint32_t headerSize = readLE32(buffer, 0);
		size_t totalPacketBytes = sizeof(uint32_t) + static_cast<size_t>(headerSize);

		if (buffer.size() < totalPacketBytes) {
			return;
		}
		const size_t typeOffset = sizeof(uint32_t) + sizeof(uint16_t);
		sl::PacketType ptype = static_cast<sl::PacketType>(read8(buffer, typeOffset));

		std::vector<uint8_t> packetBytes(buffer.begin(), buffer.begin() + totalPacketBytes);

		dataProcessorManager.routeData(packetBytes, ptype);

		buffer.erase(buffer.begin(), buffer.begin() + totalPacketBytes);
	}
}
