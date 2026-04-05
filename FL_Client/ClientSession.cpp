#include "ClientSession.hpp"
#include "IncomingQueue.hpp"
#include "OutgoingQueue.hpp"
#include "InputManager.hpp"
#include "OutputManager.hpp"
#include <iostream>
ClientSession::ClientSession(asio::ip::tcp::socket socket)
    : sessionSocket(std::move(socket)), writeWaitingTimer(sessionSocket.get_executor(), writeWaitingTime)
{
	incomingQueue = std::make_shared<IncomingQueue>();
	outgoingQueue = std::make_shared<OutgoingQueue>();
	inputManager = std::make_unique<InputManager>(incomingQueue);
	outputManager = std::make_unique<OutputManager>(outgoingQueue);
}

ClientSession::~ClientSession() = default;

void ClientSession::writeOnOutgoingData(std::vector<char>& data)
{
	outgoingQueue->push(data);
}

void ClientSession::doWrite()
{
	auto self(shared_from_this());
	bool isOutgoing = outgoingQueue->tryPop(buffer);
	if (!isOutgoing) {
		writeWaitingTimer.async_wait([this, self](const std::error_code&) {  writeWaitingTimer.expires_at(writeWaitingTimer.expiry() + writeWaitingTime);  doWrite(); });
		std::cout << "No data to write, waiting..." << std::endl;
		return;
	}

	asio::async_write(sessionSocket, asio::buffer(buffer), [this, self](std::error_code ec, size_t len) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Write continues" << std::endl;
		doWrite();
		});
}
