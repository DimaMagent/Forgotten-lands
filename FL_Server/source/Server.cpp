#include "pch.hpp"
#include "Server.hpp"
#include <filesystem>
#include <thread>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include "Session.hpp"
#include "IncomingDataManager.hpp"
#include "DataProcessorManager.hpp"
#include "ServerEntityFactory.hpp"
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
#include "DefferedFunctionStorage.hpp"


Server::Server(short port)
{
	initLogging();

	this->port = port;

	serverContext = std::make_unique<asio::io_context>();
	connectionEvents = std::make_unique<ConnectionEvents>();
	world = std::make_unique<World>(*connectionEvents);
	playerManager = std::make_unique<PlayerManager>();
	dataProcessorManager = std::make_unique<DataProcessorManager>(*playerManager);
	netManager = std::make_unique<NetManager>(*serverContext, port, *dataProcessorManager, *connectionEvents);
	entityFactory = std::make_unique<ServerEntityFactory>();
	tickTimer = std::make_shared<asio::steady_timer>(*serverContext);

	sl::DefferedFunctionStorage::init(*serverContext);
	entityFactory->initialize();
	netManager->OnAccept.addFunction([this](uint32_t token) {onClientAccept(token); });
}

Server::~Server() {
	spdlog::info("Server stopping...");
	spdlog::shutdown();
}

void Server::start() {

	spdlog::info("Server started on port {}", port);

	netManager->doAccept();
	clock.restart();
	sheduleTick();
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
	try {
		std::unique_ptr<sl::Entity> playerEntity = entityFactory->createEntity(sl::EntityType::Player);
		if (!playerEntity) { return; }

		uint32_t entityGlobalId = playerEntity->getId();
		
		playerEntity->setGlobalId(entityGlobalId);

		std::vector<uint8_t> entityData = world->addPlayerEntity(std::move(playerEntity), token);

		Packer::send<sl::net::AuthPacket>(token, entityGlobalId, entityData);
	}
	catch (std::exception& e) {
		net_logger->error("World::addPlayerEntity: token {} exception: {}", token, e.what());
		netManager->closeSession(token);
	}
}

void Server::initLogging()
{
	try {
		std::filesystem::create_directories("logs/server");

		spdlog::drop("network");
		spdlog::drop("system");

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

		system_logger = std::make_shared<spdlog::async_logger>(
			"system", sinks.begin(), sinks.end(),
			spdlog::thread_pool(), spdlog::async_overflow_policy::block);

		spdlog::register_logger(net_logger);
		spdlog::register_logger(system_logger);

		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
		spdlog::flush_every(std::chrono::seconds(3));
	}
	catch (std::exception & e) {
		std::cerr << "Server::initLogging threw the exception: " << e.what() << "\n";
	}
}

void Server::tick(float dt) {
	if (world) {
		world->update(dt);
		playerManager->tick(dt, *world);
	}
	else {
	#ifdef DEBUG
		system_logger->error("Server::tick: world is not valid, world's update and playerManager's tick cannot calls");
	#endif // DEBUG
	}
}

void Server::sheduleTick()
{
	tickTimer->expires_at(tickTimer->expiry() + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(1.0 / 60.0)));
	tickTimer->async_wait([this](const asio::error_code& ec) {
		if (ec) return;
		tick(clock.restart().asSeconds());
		sheduleTick();
		});
}
