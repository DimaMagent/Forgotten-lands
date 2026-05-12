#pragma once
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include "PacketDataTypes.hpp"
#include "PacketManager.hpp"

namespace sl::net { class Packet; }
class PlayerManager;

class DataProcessorManager {
public:
	DataProcessorManager(PlayerManager& playerManager);

	void routeData(std::vector<uint8_t>&& data, sl::net::PacketType type, uint32_t token);

	template<typename PacketT>
	void registerHandler(sl::net::PacketType type, std::function<void(const uint32_t&, const PacketT&)> handler) {
		auto key = static_cast<uint8_t>(type);
		handlers[key] = std::move([this, handler = std::move(handler)](uint32_t token, const std::vector<uint8_t>& data) {
			PacketT pkt;
			sl::net::PacketManager::read(data, pkt);
			handler(token, pkt);
		});
	}

private:
	std::unordered_map<uint8_t, std::function<void(const uint32_t&, std::vector<uint8_t>&&)>> handlers;
	PlayerManager& playerManager;
};