#include "pch.hpp"
#include "InputManager.hpp"

InputManager::InputManager(bool& isRunningFlag) : isRunningFlag(isRunningFlag)
{
}

InputManager::~InputManager() = default;

void InputManager::handleEvent(const sf::Event& event){
	if (event.type == sf::Event::Closed) {
		isRunningFlag = false;
	}
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Escape) {
			isRunningFlag = false;
			std::cout << "Input: Escape" << std::endl;
		}
		onEvent.broadcast(event);
		break;
	case sf::Event::KeyReleased:
		onEvent.broadcast(event);
		break;
	case sf::Event::MouseMoved:
		//mousePosChecker(event);
		break;
	default:
		break;
	}
}

void InputManager::mousePosChecker(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		std::cout << "Mouse position: (" << event.mouseMove.x << ", " << event.mouseMove.y << ")" << std::endl;
	}
}

