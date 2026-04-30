#pragma once
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_map>

#include "PacketDataTypes.hpp"
#include "PacketManager.hpp"

namespace sl { class Packet; }

class DataProcessorManager {
public:
	DataProcessorManager();

	void routeData(const std::vector<uint8_t>& data, sl::PacketType type);

	template<typename PacketT>
	void registerHandler(sl::PacketType type, std::function<void(const PacketT&)> handler) {
		auto key = static_cast<uint8_t>(type);
		handlers[key] = [handler](const std::vector<uint8_t>& data) {
			PacketT pkt;
			sl::PacketManager::read(data, pkt);
			handler(pkt);
		};
	}

private:
	std::unordered_map<uint8_t, std::function<void(const std::vector<uint8_t>&)>> handlers;
};