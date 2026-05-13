#include "pch.hpp"
#include "DataProcessorManager.hpp"
#include "PacketDataTypes.hpp"
#include "StatusPacket.hpp"
#include "PlayerStateManager.hpp"

DataProcessorManager::DataProcessorManager()
{
	registerHandler<sl::net::StatusPacket>(sl::net::StatusPacket::type(),
		[this](const sl::net::StatusPacket& p) {
			const auto& data = p.getData();
			auto psm = playerStateManager.lock();
			if (!psm) { return; }
			psm->recordRollback(data);
		});
}

void DataProcessorManager::routeData(std::vector<uint8_t>&& data, sl::net::PacketType type)
{
	auto it = handlers.find(static_cast<uint8_t>(type));
	if (it != handlers.end()) {
		it->second(std::move(data));
	}
	else {
		std::cerr << "Unknown packet type: " << static_cast<int>(type) << "\n";
	}
}

void DataProcessorManager::SetPlayerStateManager(std::weak_ptr<PlayerStateManager> psm)
{
	playerStateManager = psm;
}
