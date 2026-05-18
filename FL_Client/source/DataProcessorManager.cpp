#include "pch.hpp"
#include "DataProcessorManager.hpp"
#include "PacketDataTypes.hpp"
#include "StatusPacket.hpp"
#include "AuthPacket.hpp"
#include "StateManager.hpp"

DataProcessorManager::DataProcessorManager(std::weak_ptr<StateManager> manager)
{
	this->stateManager = manager;

	registerHandler<sl::net::StatusPacket>(sl::net::StatusPacket::type(),
		[this](const sl::net::StatusPacket& p) {
			const auto& data = p.getData();
			auto sm = stateManager.lock();
			if (!sm) { return; }
			sm->recordRollback(data);
		});
	registerHandler<sl::net::AuthPacket>(sl::net::AuthPacket::type(),
		[this](const sl::net::AuthPacket& p) {
			const auto& data = p.getData();
			auto sm = stateManager.lock();
			if (!sm) { return; }
			sm->auth(data);
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

