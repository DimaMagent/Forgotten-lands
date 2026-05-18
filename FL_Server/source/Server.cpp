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
#include "Packer.hpp"
#include "AuthPacket.hpp"


Server::Server(short port) : serverContext(std::make_unique<asio::io_context>()),
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
	auto timer = std::make_shared<asio::steady_timer>(*serverContext);
	sf::Clock clock;
	std::function<void()> scheduleUpdate = [&, timer]() {
		timer->expires_at(timer->expiry() + std::chrono::seconds(1/60));
		timer->async_wait([&, timer](const asio::error_code& ec) {
			if (ec) return;
			world->update(clock.restart().asSeconds());
			scheduleUpdate();
			});
		};
	scheduleUpdate();
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
	std::unique_ptr<sl::Entity> playerEntity = entityFactory->createEntity(sl::EntityType::Player);
	if (!playerEntity) { return; }
	uint32_t entityGlobalId = playerEntity->getId();
	world->addPlayerEntity(std::move(playerEntity), token);
	Packer::send<sl::net::AuthPacket>(token, entityGlobalId);
}
