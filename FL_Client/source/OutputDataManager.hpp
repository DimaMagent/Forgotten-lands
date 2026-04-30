#pragma once
#include <memory>
namespace sl {
	class Packet;
}
class ClientSession;

class OutputDataManager {
public:
	OutputDataManager(std::shared_ptr<ClientSession> session);
	void writePacket(const sl::Packet& packetData);
private:
	std::weak_ptr<ClientSession> session;
};