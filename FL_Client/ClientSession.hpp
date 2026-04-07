#pragma once
#include <memory>
#include <vector>
#include "asio.hpp"


class DataQueue;
class InputManager;
class OutputManager;

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
	ClientSession(asio::ip::tcp::socket socket);
	~ClientSession();

	void writeOnOutgoingData(std::vector<char>& data);
	void start();
private:
	asio::ip::tcp::socket sessionSocket;
	asio::strand<asio::ip::tcp::socket::executor_type> sessionStrand;
	asio::chrono::milliseconds writeWaitingTime = asio::chrono::milliseconds(500);
	std::shared_ptr<DataQueue> incomingQueue;
	std::shared_ptr<DataQueue> outgoingQueue;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<OutputManager> outputManager;

	void doWrite();
	void doRead();

};