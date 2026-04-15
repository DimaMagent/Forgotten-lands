#include "InputManager.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

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
		break;
	default:
		break;
	}
}

