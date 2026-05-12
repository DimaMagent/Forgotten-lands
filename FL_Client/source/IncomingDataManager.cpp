#include "pch.hpp"
#include "IncomingDataManager.hpp"
#include "DataQueue.hpp"
#include "PacketManager.hpp"
#include "NetUtils.hpp"
#include "Header.hpp"
#include "StatusPacket.hpp"

namespace {
	constexpr size_t WIRE_HEADER_SIZE = sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint8_t) + sizeof(uint32_t);
}

IncomingDataManager::IncomingDataManager(std::weak_ptr<sl::net::DataQueue> incQueue):
	incomingQueue(incQueue)
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
		//temporary solution
		sl::net::StatusPacket packet;

		offset = 0;

		packet.read(buffer, offset);

		sl::net::Header header = packet.getHeader();

		sl::net::StatusData data = packet.getData();

		statusDataDecipher(data.data);

		sl::net::PacketType ptype = static_cast<sl::net::PacketType>(header.getData().type);

		std::vector<uint8_t> packetBytes(buffer.begin(), buffer.begin() + totalPacketBytes);

		buffer.erase(buffer.begin(), buffer.begin() + totalPacketBytes);
	}
}

void IncomingDataManager::statusDataDecipher(const std::vector<uint8_t>& data)
{
	size_t offset = 0;
	uint32_t size = sl::net::read_uint32_t(data, offset);
	float x = (float)sl::net::read_uint32_t(data, offset);
	float y = (float)sl::net::read_uint32_t(data, offset);

	std::cout << "Received server data: " << x << " ; " << y << "\n";
}
