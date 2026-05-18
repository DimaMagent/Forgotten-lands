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

Client::Client() :
	clientContext(std::make_unique<asio::io_context>()),
	world(std::make_unique<LocalWorld>()),
	stateManager(std::make_shared<StateManager>(world->OnSetPlayerEntity)),
	dataProcessorManager(std::make_unique<DataProcessorManager>(stateManager)),
	netManager(std::make_unique<NetManager>(*clientContext, *dataProcessorManager)),
	inputManager(std::make_unique<InputManager>(isRunningFlag)), 
	entityFactory(std::make_unique<ClientEntityFactory>()),
	controller(std::make_unique<Controller>(*inputManager, *world))
{
	netManager->OnAccept.addFunction([this]() {this->onClientAccept(); });
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
		world->initializeRender(*window);
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

void Client::onClientAccept()
{
	world->setPlayerEntity(entityFactory->createEntity(sl::EntityType::Player));
}
