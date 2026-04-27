#pragma once
#include "asio\ip\tcp.hpp"
#include <memory>

class NetComponent;
class InputManager;
class World;

class Controller;
namespace asio {
	class io_context;
}
namespace sf {
	class RenderWindow;
}
namespace sl {
	class EntityFactory;
}


class Client {
public:
	Client();
	~Client();
	void start();
private:
	std::unique_ptr<asio::io_context> clientContext;
	std::unique_ptr<NetComponent> netComponent;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<sf::RenderWindow> window;
	std::unique_ptr<World> world;
	std::unique_ptr<Controller> controller;
	std::unique_ptr<sl::EntityFactory> entityFactory;
	bool isRunningFlag = false;
};