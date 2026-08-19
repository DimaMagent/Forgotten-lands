#include "pch.hpp"
#include "InputManager.hpp"

InputManager::InputManager(bool& isRunningFlag) : isRunningFlag(isRunningFlag)
{
	game_logger = spdlog::get("game");
}

InputManager::~InputManager() = default;

void InputManager::handleEvent(const sf::Event& event){
	if (event.is<sf::Event::Closed>()) {
		isRunningFlag = false;
		return;
	}
	if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
		if (keyEvent->code == sf::Keyboard::Key::Escape) {
			isRunningFlag = false;
			game_logger->info("Escape key pressed. Exiting application.");
			return;
		}	
	}
	onEvent.broadcast(event);
}

