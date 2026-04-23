#include "pch.hpp"
#include "Server.hpp"
#include "Session.hpp"
#include "IncomingDataManager.hpp"

Server::Server(asio::io_context& context, short port) : acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
	
	doAccept();
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
				incomingDataManager = std::make_shared<IncomingDataManager>(sessionPtr->getIncomingQueue());
			}
			else {
				std::cout << ec.value() << "::" << ec.message() << std::endl;
			}
			doAccept();
		});
}

