#include "pch.hpp"
#include "DataProcessorManager.hpp"

DataProcessorManager::DataProcessorManager()
{
	net_logger = spdlog::get("network");
}

void DataProcessorManager::routeData(std::vector<uint8_t>&& data, sl::net::PacketType type) const
{
	OnDataProcessed.broadcast(type, std::move(data));
}

