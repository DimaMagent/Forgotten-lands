#include "pch.hpp"
#include "Server.hpp"
#include "Session.hpp"
#include "IncomingDataManager.hpp"
#include "DataProcessorManager.hpp"
#include "NetManager.hpp"
#include "World.hpp"
#include <filesystem>

Server::Server( short port) : serverContext(std::make_unique<asio::io_context>()),
	dataProcessorManager(std::make_unique<DataProcessorManager>()),
	netManager(std::make_unique<NetManager>(*serverContext, port, *dataProcessorManager)),
	world(std::make_unique<World>())
{
}

Server::~Server() = default;

void Server::start() {
	netManager->doAccept();
	serverContext->run();
}
