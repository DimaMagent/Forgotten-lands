#pragma once
#include <memory>
#include <vector>
#include <cstdint>
#include "asio.hpp"

namespace sl {
	class DataQueue;
	class NetData;
}


class Session: public std::enable_shared_from_this<Session> {
public:
	Session(asio::ip::tcp::socket socket);
	~Session() = default;
	void start() { doRead(); }
	void writeOnOutgoingData(std::vector<uint8_t>& data);
	std::shared_ptr<sl::DataQueue> getIncomingQueue() const { return incomingQueue; }
private:
	asio::ip::tcp::socket sessionSocket;
	asio::strand<asio::ip::tcp::socket::executor_type> sessionStrand;
	std::shared_ptr<sl::DataQueue> incomingQueue;
	std::shared_ptr<sl::DataQueue> outgoingQueue;

	void doRead();
	void doWrite();
};