#include "Client.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "NetComponent.hpp"
#include "InputManager.hpp"
#include "World.hpp"
#include "CharacterFactory.hpp"
#include "Character.hpp"
#include "Controller.hpp"
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
		window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "FL_Client.exe", sf::Style::Default); // sf::Style::Fullscreen
		window->setVerticalSyncEnabled(true);
		world = std::make_unique<World>(*window);
		controller = std::make_unique<Controller>(*inputManager, *world);
		world->setPlayerCharacter(std::move(characterFactory->createCharacter(CharacterType::Player)));
		sf::Clock timer;
		for (;;) {

			//sf::Time test = sf::seconds(1.f / 60.f);
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
			world->update(timer.restart().asSeconds());
			window->clear(sf::Color::Black);
			world->render();
			window->display();
		}
		isRunningFlag = false;
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
