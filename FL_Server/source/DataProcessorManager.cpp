#include "pch.hpp"
#include "DataProcessorManager.hpp"
#include "PacketDataTypes.hpp"
#include "InputStatePacket.hpp"
#include "PlayerManager.hpp"

DataProcessorManager::DataProcessorManager(PlayerManager& playerManager): playerManager(playerManager)
{
	registerHandler<sl::net::InputStatePacket>(sl::net::PT_InputState,
    [this](const uint32_t& token, const sl::net::InputStatePacket& p){
        const auto& data = p.getData();

        std::cout << "Received InputState packet, state: " << static_cast<int>(data.inputState) << "is pressing: " << data.pressingFlag << "\n";
		this->playerManager.updatePlayerInputState(token, data.inputState, data.pressingFlag);
    });
}

void DataProcessorManager::routeData(const std::vector<uint8_t>& data, sl::net::PacketType type, uint32_t token)
{
	auto it = handlers.find(static_cast<uint8_t>(type));
	if (it != handlers.end()) {
		it->second(token, data);
	}
	else {
		std::cerr << "Unknown packet type: " << static_cast<int>(type) << "\n";
	}
}
