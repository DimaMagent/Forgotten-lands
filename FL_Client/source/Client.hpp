#pragma once
#include "asio\ip\tcp.hpp"
#include <memory>

class NetManager;
class InputManager;
class LocalWorld;

class Controller;
namespace asio {
	class io_context;
}
namespace sf {
	class RenderWindow;
}

class ClientEntityFactory;
class DataProcessorManager;
class StateManager;

//TODO: It is necessary to structure the client's classes and their dependencies.

class Client {
public:
	Client();
	~Client();
	void start();
private:
	//loggers
	std::shared_ptr<spdlog::logger> net_logger;
	std::shared_ptr<spdlog::logger> game_logger;
	std::shared_ptr<spdlog::logger> load_logger;

	std::unique_ptr<asio::io_context> clientContext;
	std::unique_ptr<sf::RenderWindow> window;
	// TODO: It might be worth moving the factory to LocalWorld
	std::shared_ptr<ClientEntityFactory> entityFactory;
	std::unique_ptr<LocalWorld> world;
	std::unique_ptr<DataProcessorManager> dataProcessorManager;
	std::unique_ptr<NetManager> netManager;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<Controller> controller;

	bool isRunningFlag = false;

	void whenClientAccepted();

	void initLogging();
};