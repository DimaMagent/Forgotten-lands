#pragma once
#include <memory>
#include <vector>
#include "asio.hpp"


class IncomingQueue;
class OutgoingQueue;
class InputManager;
class OutputManager;

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
	ClientSession(asio::ip::tcp::socket socket);
	~ClientSession();

	void writeOnOutgoingData(std::vector<char>& data);
	void start() { doWrite(); }
private:
	asio::ip::tcp::socket sessionSocket;
	std::vector<char> buffer;
	asio::chrono::milliseconds writeWaitingTime = asio::chrono::milliseconds(500);
	asio::steady_timer writeWaitingTimer;
	std::shared_ptr<IncomingQueue> incomingQueue;
	std::shared_ptr<OutgoingQueue> outgoingQueue;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<OutputManager> outputManager;

	void doWrite();

};