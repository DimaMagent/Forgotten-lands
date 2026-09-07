#include "pch.hpp"
#include "DataProcessorManager.hpp"

DataProcessorManager::DataProcessorManager()
{
	logger = spdlog::get("network");
}

void DataProcessorManager::routeData(std::vector<uint8_t>&& data, sl::net::PacketType type, uint32_t token) const
{
	OnDataProcessed.broadcast(type, std::move(data), token);
}
