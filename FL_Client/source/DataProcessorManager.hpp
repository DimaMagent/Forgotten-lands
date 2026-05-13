#pragma once
#include <vector>
#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include "PacketDataTypes.hpp"
#include "PacketManager.hpp"

namespace sl::net { class Packet; }
class PlayerStateManager;

class DataProcessorManager {
public:
	DataProcessorManager();

	void routeData(std::vector<uint8_t>&& data, sl::net::PacketType type);

	// temporary solution
	void SetPlayerStateManager(std::weak_ptr<PlayerStateManager> psm);

	template<typename PacketT>
	void registerHandler(sl::net::PacketType type, std::function<void( const PacketT&)> handler) {
		auto key = static_cast<uint8_t>(type);
		handlers[key] = std::move([this, handler = std::move(handler)](const std::vector<uint8_t>& data) {
			PacketT pkt;
			sl::net::PacketManager::read(data, pkt);
			handler(pkt);
			});
	}

private:
	std::unordered_map<uint8_t, std::function<void(std::vector<uint8_t>&&)>> handlers;
	std::weak_ptr<PlayerStateManager> playerStateManager;
};