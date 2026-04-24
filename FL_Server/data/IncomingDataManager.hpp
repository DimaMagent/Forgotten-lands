#pragma once
#include <memory>
#include <vector>
#include <cstdint>

class DataProcessorManager;
namespace sl {
	class DataQueue;
}

class IncomingDataManager {
public:
	IncomingDataManager(std::shared_ptr<sl::DataQueue> incQueue, DataProcessorManager& dpm);
private:
	std::weak_ptr<sl::DataQueue> incomingQueue;
	DataProcessorManager& dataProcessorManager;

	void assemblePacket();
	std::vector<uint8_t> buffer;
};