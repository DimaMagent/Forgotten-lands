#include "pch.hpp"
#include "InputManager.hpp"

InputManager::InputManager(bool& isRunningFlag) : isRunningFlag(isRunningFlag)
{
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
			std::cout << "Input: Escape" << std::endl;
		}
		onEvent.broadcast(event);
		return;
	}
	if (event.is<sf::Event::KeyReleased>()) {
		onEvent.broadcast(event);
		return;
	}
	// if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
	//     mousePosChecker(*mouseMoved);
	// }}
}

void InputManager::mousePosChecker(const sf::Event::MouseMoved& event) {
	std::cout << "Mouse position: (" << event.position.x << ", " << event.position.y << ")" << std::endl;
}

