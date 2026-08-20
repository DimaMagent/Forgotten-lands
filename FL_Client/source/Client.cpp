#include "pch.hpp"
#include "Client.hpp"
#include "NetManager.hpp"
#include "InputManager.hpp"
#include "LocalWorld.hpp"
#include "ClientEntityFactory.hpp"
#include "Entity.hpp"
#include "Controller.hpp"
#include "DataProcessorManager.hpp"
#include "StateManager.hpp"
#include "PlayerIntentManager.hpp"
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h> 

Client::Client() 
{
	initLogging();

	clientContext = std::make_unique<asio::io_context>();
	window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "FL_Client.exe", sf::State::Windowed); // sf::State::Fullscreen
	entityFactory = std::make_shared<ClientEntityFactory>();
	world = std::make_unique<LocalWorld>(entityFactory, *window);
	dataProcessorManager = std::make_unique<DataProcessorManager>(world->getStateManager());
	netManager = std::make_unique<NetManager>(*clientContext, *dataProcessorManager);
	inputManager = std::make_unique<InputManager>(isRunningFlag);
	controller = std::make_unique<Controller>(*inputManager, world->OnSetPlayerEntity);
	playerIntentManager = std::make_unique<PlayerIntentManager>(
		controller->onNewAction,
		controller->onSetMovementDirection,
		world->OnSetPlayerEntity
	);

	netManager->OnAccept.addFunction([this]() {this->whenClientAccepted(); });
	entityFactory->initialize();
	window->setVerticalSyncEnabled(true);
}

Client::~Client() {
	spdlog::info("Client shutdown...");
	spdlog::shutdown();
}

void Client::start()
{
	try {
		isRunningFlag = true;
		netManager->doConnect();
		std::thread ClientThread([this]() {clientContext->run(); });
		sf::Clock timer;
		for (;;) {
			while (const std::optional<sf::Event> event = window->pollEvent()) {
				inputManager->handleEvent(event.value());
			}
			if (!isRunningFlag) {
				window->close();
				break;
			}
			if (!window->isOpen()) {
				break;
			}
			tick(timer.restart().asSeconds());
			window->clear(sf::Color::Black);
			world->render();
			window->display();
		}
		isRunningFlag = false;
		clientContext->stop();
		ClientThread.join();
		return;
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		std::cin.get();
	}
}

void Client::whenClientAccepted(){}

void Client::initLogging() {
	try {
		spdlog::init_thread_pool(8192, 1);

		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
			"logs/client/client.log", 1024 * 1024 * 10, 5);

		console_sink->set_level(spdlog::level::warn);
		file_sink->set_level(spdlog::level::trace);
		std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };

		net_logger = std::make_shared<spdlog::async_logger>(
			"network", sinks.begin(), sinks.end(),
			spdlog::thread_pool(), spdlog::async_overflow_policy::block);

		game_logger = std::make_shared<spdlog::async_logger>(
			"game", sinks.begin(), sinks.end(),
			spdlog::thread_pool(), spdlog::async_overflow_policy::block);

		system_logger = std::make_shared<spdlog::async_logger>(
			"system", sinks.begin(), sinks.end(),
			spdlog::thread_pool(), spdlog::async_overflow_policy::block);

		load_logger = std::make_shared<spdlog::async_logger>(
			"load", sinks.begin(), sinks.end(),
			spdlog::thread_pool(), spdlog::async_overflow_policy::block);

		spdlog::register_logger(net_logger);
		spdlog::register_logger(game_logger);
		spdlog::register_logger(load_logger);
		spdlog::register_logger(system_logger);

		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
		spdlog::flush_every(std::chrono::seconds(3));
	}
	catch (std::exception& e) {
		spdlog::error("Client::initLogging threw the exception: {}", e.what());
	}
}

void Client::tick(float dt) {
	if (world) {
		world->update(dt);
		playerIntentManager->tick(dt, *world);
	}
	else {
		#ifdef DEBUG
			system_logger->error("Client::tick: world is not valid, world's update and playerIntentManager's tick cannot calls");
		#endif // DEBUG
	}
}
