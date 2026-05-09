#include "pch.hpp"
#include "Server.hpp"
#include <filesystem>
#include <thread>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include "Session.hpp"
#include "IncomingDataManager.hpp"
#include "DataProcessorManager.hpp"
#include "EntityFactory.hpp"
#include "NetManager.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "PlayerManager.hpp"


Server::Server( short port) : serverContext(std::make_unique<asio::io_context>()),
	world(std::make_unique<World>()),
	playerManager(std::make_unique<PlayerManager>(*world)),
	dataProcessorManager(std::make_unique<DataProcessorManager>(*playerManager)),
	netManager(std::make_unique<NetManager>(*serverContext, port, *dataProcessorManager)),
	entityFactory(std::make_unique<sl::EntityFactory>())
{
	entityFactory->initialize();
	netManager->OnAccept.addFunction([this](uint32_t token) {onClientAccept(token); });
}

Server::~Server() = default;

void Server::start() {
	netManager->doAccept();
	sf::Clock timer;
	std::function<void()> scheduleUpdate = [&]() {
		world->update(timer.restart().asSeconds());
		asio::post(*serverContext, scheduleUpdate);
		};

	asio::post(*serverContext, scheduleUpdate);
	serverContext->run();
	////std::thread ServerThread([this]() {serverContext->run(); });
	//sf::Clock timer;
	//for (;;) {
	//	world->update(timer.restart().asSeconds());
	//}
	////serverContext->stop();
	////ServerThread.join();
}

void Server::onClientAccept(uint32_t token)
{
	world->addPlayerEntity(std::move(entityFactory->createEntity(sl::EntityType::Player)), token);
}
