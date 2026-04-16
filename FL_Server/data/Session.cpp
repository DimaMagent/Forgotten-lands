#include "Session.hpp"
#include <iostream>
#include <string>
#include "DataQueue.hpp"

Session::Session(asio::ip::tcp::socket socket) :
	sessionSocket(std::move(socket)), sessionStrand(asio::make_strand(sessionSocket.get_executor()))
{
	incomingQueue = std::make_shared<sl::DataQueue>();
	outgoingQueue = std::make_shared<sl::DataQueue>();
}

void Session::writeOnOutgoingData(sl::NetData& data)
{
	auto self = shared_from_this();
	outgoingQueue->push(data);
	asio::post(sessionStrand, [self]() { self->doWrite(); });
}

void Session::doRead()
{
	auto self(shared_from_this());
	std::shared_ptr<sl::NetData> localBuffer = std::make_shared<sl::NetData>(8192u);
	sessionSocket.async_read_some(asio::buffer(localBuffer->getData()), asio::bind_executor(sessionStrand, [this, self, localBuffer](std::error_code ec, size_t len) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Received: " << std::string(localBuffer->getData().data(), len) << std::endl;
		incomingQueue->push(*localBuffer);
		doRead();
		}));

}

void Session::doWrite()
{
	auto self(shared_from_this());
	std::shared_ptr<sl::NetData> localBuffer = std::make_shared<sl::NetData>(1024u);
	bool isOutgoing = outgoingQueue->tryPop(*localBuffer);
	if (!isOutgoing) {
		std::cout << "No data to write, waiting..." << std::endl;
		return;
	}

	asio::async_write(sessionSocket, asio::buffer(localBuffer->getData()), asio::bind_executor(sessionStrand, [this, self, localBuffer](std::error_code ec, size_t len) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Write continues" << std::endl;
		doWrite();
		}));
}
