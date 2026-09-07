#include "pch.hpp"
#include "DataProcessorManager.hpp"
#include "network/packets/PacketDataTypes.hpp"
#include "network/packets/PlayerIntentionsPacket.hpp"
#include "system/playerSystems/PlayerManager.hpp"

DataProcessorManager::DataProcessorManager()
{
	logger = spdlog::get("network");
}

void DataProcessorManager::routeData(std::vector<uint8_t>&& data, sl::net::PacketType type, uint32_t token) const
{
	OnDataProcessed.broadcast(type, std::move(data), token);
}
