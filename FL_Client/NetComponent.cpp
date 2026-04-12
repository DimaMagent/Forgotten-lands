#include "NetComponent.hpp"
#include "asio.hpp"
#include "ClientSession.hpp"
#include <iostream>
#include <vector>
#include "NetData.hpp"

NetComponent::NetComponent(asio::io_context& context) : socket(context), endpoint(asio::ip::make_address(serverAddress), serverPort) {}

bool NetComponent::tryWrite(NetData& data)
{
	if (auto sessionPtr = session.lock()) {
		sessionPtr->writeOnOutgoingData(data);
		return true;
	}
	return false;
}

void NetComponent::doConnect()
{
	std::cout << "client started connecting address: " << serverAddress << ":" << serverPort << std::endl;

	socket.async_connect(endpoint, [this](std::error_code ec) {
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