#include "Server.hpp"
#include "asio.hpp"
#include <iostream>
#include "Session.hpp"
#include "NetData.hpp"

Server::Server(asio::io_context& context, short port) : acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{

	doAccept();
}

bool Server::tryWrite(NetData& data)
{
	if (auto sessionPtr = session.lock()) {
		sessionPtr->writeOnOutgoingData(data);
		return true;
	}
	return false;
}

void Server::doAccept()
{
	acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec) {
				std::cout << "Client connected" << std::endl;
				std::shared_ptr<Session> sessionPtr = std::make_shared<Session>(std::move(socket));
				sessionPtr->start();
				session = sessionPtr;
				NetData data(std::vector<char>{ 'H', 'e', 'l', 'l', 'o', ' ', 'f', 'r', 'o', 'm', ' ', 's', 'e', 'r', 'v', 'e', 'r' });
				tryWrite(data);
			}
			else {
				std::cout << ec.value() << "::" << ec.message() << std::endl;
			}
			doAccept();
		});
}

