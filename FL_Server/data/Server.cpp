#include "pch.hpp"
#include "Server.hpp"
#include "Session.hpp"
#include "IncomingDataManager.hpp"
#include "DataProcessorManager.hpp"

Server::Server(asio::io_context& context, short port) : acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
	dataProcessorManager = std::make_unique<DataProcessorManager>();
	doAccept();
}

Server::~Server() = default;

void Server::doAccept()
{
	acceptor.async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec) {
				std::cout << "Client connected" << std::endl;
				std::shared_ptr<Session> sessionPtr = std::make_shared<Session>(std::move(socket));
				sessionPtr->start();
				sessions.push_back(sessionPtr);
				incomingDataManager = std::make_shared<IncomingDataManager>(sessionPtr->getIncomingQueue(), *dataProcessorManager);
			}
			else {
				std::cout << ec.value() << "::" << ec.message() << std::endl;
			}
			doAccept();
		});
}

