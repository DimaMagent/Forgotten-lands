#pragma once
#include <memory>
#include <vector>

namespace sl::net {
	class DataQueue;
}
class DataProcessorManager;

namespace sl {
	class DelegateToken;
}

class IncomingDataManager {
public:
	IncomingDataManager(std::weak_ptr<sl::net::DataQueue> incQueue, DataProcessorManager& dpm);
private:
	std::shared_ptr<spdlog::logger> logger;

	std::weak_ptr<sl::net::DataQueue> incomingQueue;
	std::vector<uint8_t> buffer;
	std::vector<std::shared_ptr<sl::DelegateToken>> tokens;
	DataProcessorManager& dataProcessorManager;
	void onDataPushed();
	void assemblePacket();
};