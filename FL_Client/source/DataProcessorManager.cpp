#include "pch.hpp"
#include "DataProcessorManager.hpp"
#include "PacketDataTypes.hpp"
#include "StatusPacket.hpp"

DataProcessorManager::DataProcessorManager()
{
	registerHandler<sl::net::StatusPacket>(sl::net::StatusPacket::type(),
		[this](const sl::net::StatusPacket& p) {
			const auto& data = p.getData();
			size_t offset = 0;
			uint32_t size = sl::net::read_uint32_t(data.data, offset);
			float x = (float)sl::net::read_uint32_t(data.data, offset);
			float y = (float)sl::net::read_uint32_t(data.data, offset);

			std::cout << "Received server data: " << x << " ; " << y << "\n";
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