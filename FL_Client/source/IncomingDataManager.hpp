#pragma once
#include <memory>

namespace sl::net {
	class DataQueue;
}

class IncomingDataManager {
public:
	IncomingDataManager(std::weak_ptr<sl::net::DataQueue> incQueue);
private:
	std::weak_ptr<sl::net::DataQueue> incomingQueue;
	std::vector<uint8_t> buffer;
	void onDataPushed();
	void assemblePacket();
	//temporary solution
	void statusDataDecipher(const std::vector<uint8_t>& data);
};