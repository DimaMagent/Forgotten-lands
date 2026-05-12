#include "pch.hpp"
#include "IncomingDataManager.hpp"
#include "DataQueue.hpp"
#include "PacketManager.hpp"
#include "NetUtils.hpp"
#include "Header.hpp"
#include "StatusPacket.hpp"
#include "DataProcessorManager.hpp"

namespace {
	constexpr size_t WIRE_HEADER_SIZE = sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint8_t) + sizeof(uint32_t);
}

IncomingDataManager::IncomingDataManager(std::weak_ptr<sl::net::DataQueue> incQueue, DataProcessorManager& dpm):
	incomingQueue(incQueue), dataProcessorManager(dpm)
{
	if (auto iq = incQueue.lock()) {
		iq->onDataPushed.addFunction([this]() { onDataPushed(); });
	}
}

void IncomingDataManager::onDataPushed()
{
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
}

void IncomingDataManager::assemblePacket() {
	while (buffer.size() >= WIRE_HEADER_SIZE) {
		size_t offset = 0;

		uint32_t headerSize = sl::net::read_uint32_t(buffer, offset);

		uint32_t totalPacketBytes = sizeof(uint32_t) + headerSize;

		if (buffer.size() < totalPacketBytes) {
			return;
		}
		sl::net::Header header;

		offset = 0;

		header.read(buffer, offset);

		sl::net::PacketType ptype = static_cast<sl::net::PacketType>(header.getData().type);

		std::vector<uint8_t> packetBytes(buffer.begin(), buffer.begin() + totalPacketBytes);

		dataProcessorManager.routeData(std::move(packetBytes), ptype);

		buffer.erase(buffer.begin(), buffer.begin() + totalPacketBytes);
	}
}