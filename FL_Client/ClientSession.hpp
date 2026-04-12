#pragma once
#include <memory>
#include <vector>
#include "asio\ip\tcp.hpp"


class DataQueue;
class InputDataManager;
class OutputDataManager;
class NetData;

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
	ClientSession(asio::ip::tcp::socket socket);
	~ClientSession();

	void writeOnOutgoingData(NetData& data);
	void start();
private:
	asio::ip::tcp::socket sessionSocket;
	asio::strand<asio::ip::tcp::socket::executor_type> sessionStrand;
	std::shared_ptr<DataQueue> incomingQueue;
	std::shared_ptr<DataQueue> outgoingQueue;
	std::unique_ptr<InputDataManager> inputManager;
	std::unique_ptr<OutputDataManager> outputManager;

	void doWrite();
	void doRead();

};