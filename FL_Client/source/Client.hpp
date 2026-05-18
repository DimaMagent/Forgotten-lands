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

//TODO: необходимо зан€тьс€ структуризацией классов клиента и их зависимоcт€ми

class Client {
public:
	Client();
	~Client();
	void start();
private:
	std::unique_ptr<asio::io_context> clientContext;
	// ¬озможно, стоит переместить фабрику в LocalWorld
	std::shared_ptr<ClientEntityFactory> entityFactory;
	std::unique_ptr<LocalWorld> world;
	std::unique_ptr<DataProcessorManager> dataProcessorManager;
	std::unique_ptr<NetManager> netManager;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<Controller> controller;

	bool isRunningFlag = false;

	void whenClientAccepted();
};