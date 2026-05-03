#include "pch.hpp"
#include "DataProcessorManager.hpp"
#include "PacketDataTypes.hpp"
#include "InputStatePacket.hpp"

DataProcessorManager::DataProcessorManager()
{
	registerHandler<sl::InputStatePacket>(sl::PT_InputState, std::function<void(const sl::InputStatePacket&)>(
    [](const sl::InputStatePacket& p){
        const auto& data = p.getData();
        std::cout << "Received InputState packet, state: " << static_cast<int>(data.inputState) << "is pressing: " << data.pressingFlag << "\n";
        // TODO: делегировать конкретной подсистеме (GameState, PlayerManager и т.д.)
    }
));
}

void DataProcessorManager::routeData(const std::vector<uint8_t>& data, sl::PacketType type)
{
	auto it = handlers.find(static_cast<uint8_t>(type));
	if (it != handlers.end()) {
		it->second(data);
	}
	else {
		std::cerr << "Unknown packet type: " << static_cast<int>(type) << "\n";
	}
}
