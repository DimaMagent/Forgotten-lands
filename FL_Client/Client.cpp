#include "Client.hpp"
#include <iostream>
#include "ClientSession.hpp"
#include <vector>
#include "NetData.hpp"


Client::Client(asio::io_context& context, asio::ip::tcp::endpoint endpoint) : socket(context)
{
	doConnect(endpoint);
}

bool Client::tryWrite(NetData& data)
{
	if (auto sessionPtr = session.lock()) {
		sessionPtr->writeOnOutgoingData(data);
		return true;
	}
	return false;
}

void Client::doConnect(asio::ip::tcp::endpoint connectEndpoint)
{
	socket.async_connect(connectEndpoint, [this](std::error_code ec) {
		if (ec) {
			std::cout << ec.value() << "::" << ec.message() << std::endl;
			return;
		}
		std::cout << "Connect to server" << std::endl;
		std::shared_ptr<ClientSession> sessionPtr = std::make_shared<ClientSession>(std::move(socket));
		session = sessionPtr;
		sessionPtr->start();
		NetData data(std::vector<char>{ 'H', 'e', 'l', 'l', 'o', ' ', 'f', 'r', 'o', 'm', ' ', 'c', 'l', 'i', 'e', 'n', 't'});
		tryWrite(data);
		});
}
