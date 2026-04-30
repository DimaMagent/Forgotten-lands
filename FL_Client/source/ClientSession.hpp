#pragma once
#include <memory>
#include <vector>
#include "asio\ip\tcp.hpp"

namespace sl {
	class DataQueue;
}
class IncomingDataManager;
class OutputDataManager;


class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
	ClientSession(asio::ip::tcp::socket socket);
	~ClientSession();

	void writeOnOutgoingData(std::vector<uint8_t>& data);
	void start();
	std::shared_ptr<sl::DataQueue> getIncomingQueue() { return incomingQueue; }
	std::shared_ptr<sl::DataQueue> getOutgoingQueue() { return outgoingQueue; }
private:
	asio::ip::tcp::socket sessionSocket;
	asio::strand<asio::ip::tcp::socket::executor_type> sessionStrand;
	std::shared_ptr<sl::DataQueue> incomingQueue;
	std::shared_ptr<sl::DataQueue> outgoingQueue;

	void doWrite();
	void doRead();

};