#pragma once
#include <memory>
#include <vector>
#include <cstdint>
#include "LockFreeDelegate.hpp"

class DataProcessorManager;
namespace sl::net {
	class DataQueue;
}

class IncomingDataManager {
public:
	IncomingDataManager(std::weak_ptr<sl::net::DataQueue> incQueue, DataProcessorManager& dpm, uint32_t token);
	sl::LockFreeDelegate<> OnWrongData;
private:
	std::weak_ptr<sl::net::DataQueue> incomingQueue;
	DataProcessorManager& dataProcessorManager;

	std::vector<uint8_t> buffer;
	uint32_t token;

	void assemblePacket();
	void onDataPushed();
};