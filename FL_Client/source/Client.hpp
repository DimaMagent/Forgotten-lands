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

//TODO: необходимо заняться структуризацией классов клиента и их зависимоcтями

class Client {
public:
	Client();
	~Client();
	void start();
private:
	std::unique_ptr<asio::io_context> clientContext;
	std::unique_ptr<LocalWorld> world;
	std::shared_ptr<StateManager> stateManager;
	std::unique_ptr<DataProcessorManager> dataProcessorManager;
	std::unique_ptr<NetManager> netManager;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<Controller> controller;

	std::unique_ptr<ClientEntityFactory> entityFactory;
	bool isRunningFlag = false;

	void onClientAccept();
};