#include "pch.hpp"
#include "Client.hpp"
#include "NetManager.hpp"
#include "InputManager.hpp"
#include "LocalWorld.hpp"
#include "ClientEntityFactory.hpp"
#include "Entity.hpp"
#include "Controller.hpp"

Client::Client() :
	clientContext(std::make_unique<asio::io_context>()), netManager(std::make_unique<NetManager>(*clientContext)),
	inputManager(std::make_unique<InputManager>(isRunningFlag)), 
	entityFactory(std::make_unique<ClientEntityFactory>())
{
	entityFactory->initialize();
}

Client::~Client() = default;

void Client::start()
{
	try {
		setlocale(LC_ALL, "Russian");
		isRunningFlag = true;
		netManager->doConnect();
		std::thread ClientThread([this]() {clientContext->run(); });
		window = std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), "FL_Client.exe", sf::State::Windowed); // sf::State::Fullscreen
		window->setVerticalSyncEnabled(true);
		world = std::make_unique<LocalWorld>(*window);
		controller = std::make_unique<Controller>(*inputManager, *world);
		world->setPlayerEntity(entityFactory->createEntity(sl::EntityType::Player));
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
