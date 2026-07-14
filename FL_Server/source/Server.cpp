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
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h> 
#include "connectionEvents.hpp"


Server::Server(short port)
{
	initLogging();

	serverContext = std::make_unique<asio::io_context>();
	connectionEvents = std::make_unique<ConnectionEvents>();
	world = std::make_unique<World>(*connectionEvents);
	playerManager = std::make_unique<PlayerManager>(*world);
	dataProcessorManager = std::make_unique<DataProcessorManager>(*playerManager);
	netManager = std::make_unique<NetManager>(*serverContext, port, *dataProcessorManager, *connectionEvents);
	entityFactory = std::make_unique<sl::EntityFactory>();

	entityFactory->initialize();
	netManager->OnAccept.addFunction([this](uint32_t token) {onClientAccept(token); });
}

Server::~Server() {
	spdlog::info("Server stopping...");
	spdlog::shutdown();
}

void Server::start() {

	spdlog::info("Server started on port 2001");

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

void Server::initLogging()
{
	spdlog::init_thread_pool(8192, 1);

	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
		"logs/server/server.log", 1024 * 1024 * 10, 5);

	console_sink->set_level(spdlog::level::warn);
	file_sink->set_level(spdlog::level::trace);
	std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };

	net_logger = std::make_shared<spdlog::async_logger>(
		"network", sinks.begin(), sinks.end(),
		spdlog::thread_pool(), spdlog::async_overflow_policy::block);

	spdlog::register_logger(net_logger);

	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
	spdlog::flush_every(std::chrono::seconds(3));
}
