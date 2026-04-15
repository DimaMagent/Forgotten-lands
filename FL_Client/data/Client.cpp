#include "Client.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "NetComponent.hpp"
#include "InputManager.hpp"
#include "World.hpp"
#include "CharacterFactory.hpp"
#include "Character.hpp"
#include "asio.hpp"

Client::Client() :
	clientContext(std::make_unique<asio::io_context>()), netComponent(std::make_unique<NetComponent>(*clientContext)),
	inputManager(std::make_unique<InputManager>(isRunningFlag)), characterFactory(std::make_unique<CharacterFactory>()) {}

Client::~Client() = default;

void Client::start()
{
	try {
		setlocale(LC_ALL, "Russian");
		isRunningFlag = true;
		netComponent->doConnect();
		std::thread ClientThread([this]() {clientContext->run(); });
		window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "FL_Client.exe", sf::Style::Fullscreen);
		world = std::make_unique<World>(*window);
		world->addEntity(characterFactory->createCharacter(CharacterType::Player));
		for (;;) {
			window->display();
			sf::Event event;
			while (window->pollEvent(event)) {
				inputManager->handleEvent(event);
			}
			if (!isRunningFlag) {
				window->close();
				break;
			}
			if (!window->isOpen()) {
				break;
			}
			world->render();
		}
		clientContext->stop();
		ClientThread.join();
		std::cout << "Client stopped" << std::endl;
		return;
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		std::cin.get();
	}
}