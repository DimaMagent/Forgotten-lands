#pragma once
#include "asio\ip\tcp.hpp"
#include <memory>

class NetComponent;
class InputManager;
namespace asio {
	class io_context;
}
namespace sf {
	class RenderWindow;
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
	bool isRunningFlag = false;
};