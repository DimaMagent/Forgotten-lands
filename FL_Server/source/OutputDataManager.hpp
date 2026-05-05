#pragma once
#include <memory>
#include <unordered_map>

namespace sl::net {
	class Packet;
}
class Session;

class OutputDataManager {
public:
	OutputDataManager(std::unordered_map<uint32_t, std::weak_ptr<Session>>& sessions);
	void writePacket(const sl::net::Packet& packetData, uint32_t token);
private:
	std::unordered_map<uint32_t, std::weak_ptr<Session>>& tokenToSessions;
};