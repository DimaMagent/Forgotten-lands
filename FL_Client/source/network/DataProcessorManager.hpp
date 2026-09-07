#pragma once
#include <vector>
#include "DataProcessorTypes.hpp"

namespace sl::net { class Packet; }
class StateManager;

class DataProcessorManager {
public:
	DataProcessedDelegate OnDataProcessed;

	DataProcessorManager();

	void routeData(std::vector<uint8_t>&& data, sl::net::PacketType type) const;

private:
	std::shared_ptr<spdlog::logger> net_logger;
};