#pragma once
#include <memory>
#include "asio.hpp"

class DataQueue;
class NetData;

class Session: public std::enable_shared_from_this<Session> {
public:
	Session(asio::ip::tcp::socket socket);
	~Session() = default;
	void start() { doRead(); }
	void writeOnOutgoingData(NetData& data);
private:
	asio::ip::tcp::socket sessionSocket;
	asio::strand<asio::ip::tcp::socket::executor_type> sessionStrand;
	std::shared_ptr<DataQueue> incomingQueue;
	std::shared_ptr<DataQueue> outgoingQueue;

	void doRead();
	void doWrite();
};