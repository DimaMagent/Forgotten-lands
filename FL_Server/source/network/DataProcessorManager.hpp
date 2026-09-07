#pragma once
#include "DataProcessorTypes.hpp"

namespace sl::net { class Packet; }
class PlayerManager;

class DataProcessorManager {
public:
	DataProcessedDelegate OnDataProcessed;
	DataProcessorManager();

	void routeData(std::vector<uint8_t>&& data, sl::net::PacketType type, uint32_t token) const;

private:
	//loggers
	std::shared_ptr<spdlog::logger> logger;
};