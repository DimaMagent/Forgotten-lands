#include "pch.hpp"
#include "IncomingDataManager.hpp"
#include "DataQueue.hpp"
#include "PacketManager.hpp"
#include "Packet.hpp"
#include "Header.hpp"

IncomingDataManager::IncomingDataManager(std::shared_ptr<sl::DataQueue> incQueue) : incomingQueue(incQueue) 
{
	incQueue->onDataPushed.addFunction([this]() {
		if (auto queue = incomingQueue.lock()) {
			std::vector<uint8_t> data;
			while (queue->tryPop(data)) {
				std::cout << "Data received in IncomingDataManager, size: " << sizeof(data) << "\n";
			}
		}
		else {
			std::cerr << "Failed to process incoming data: DataQueue is no longer available." << "\n";
		}
		});
}

void IncomingDataManager::assemblePacket(const std::vector<uint8_t>& data)
{
	/*
	size_t offset = 0;
	std::optional<sl::Header> header = sl::Packet::readHeader(data, offset);
	if (header) {
	}
	*/
}
