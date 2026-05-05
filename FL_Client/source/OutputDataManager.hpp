#pragma once
#include <memory>
namespace sl::net {
	class Packet;
}
class ClientSession;

class OutputDataManager {
public:
	OutputDataManager(std::weak_ptr<ClientSession> session);
	void writePacket(const sl::net::Packet& packetData);
private:
	std::weak_ptr<ClientSession> session;
};