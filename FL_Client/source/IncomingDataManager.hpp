#pragma once
#include <memory>
#include <vector>

namespace sl::net {
	class DataQueue;
}
class DataProcessorManager;

class IncomingDataManager {
public:
	IncomingDataManager(std::weak_ptr<sl::net::DataQueue> incQueue, DataProcessorManager& dpm);
private:
	std::weak_ptr<sl::net::DataQueue> incomingQueue;
	std::vector<uint8_t> buffer;
	DataProcessorManager& dataProcessorManager;
	void onDataPushed();
	void assemblePacket();
};