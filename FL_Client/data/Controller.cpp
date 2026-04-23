#include "pch.hpp"
#include "Controller.hpp"
#include "InputManager.hpp"
#include "World.hpp"
#include "Character.hpp"
#include "Packer.hpp"

Controller::Controller(InputManager& im, World& world)
{
	im.onEvent.addFunction([this](const sf::Event& event) { onEvent(event); });
	world.OnSetPlayerEntity.addFunction([this](const std::weak_ptr<Character> playerCharacter) { OnPlayerCharacterSet(playerCharacter); });
}

void Controller::onEvent(const sf::Event& event) {
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (auto it = keyBindings.find(event.key.code); it != keyBindings.end()) {
			std::cout << "Executed action for key: " << event.key.code << std::endl;
			reverseInputMultiplier = 1;
			Packer::packageInputStatePacket(it->second());
		}
		break;
	case sf::Event::KeyReleased:
		if (auto it = keyBindings.find(event.key.code); it != keyBindings.end()) {
			std::cout << "Released action for key: " << event.key.code << std::endl;
			reverseInputMultiplier = -1;
			Packer::packageInputStatePacket(it->second());
		}
		break;
	default:
		break;
	}
}

void Controller::OnPlayerCharacterSet(std::weak_ptr<Character> playerCharacter)
{
	if (playerCharacter.expired()) {
		std::cout << "Received expired player character reference." << std::endl;
		return;
	}
	std::cout << "Player character set in controller." << std::endl;
	this->playerCharacter = playerCharacter;
	InitKeyBindings();
}

void Controller::InitKeyBindings(){
	keyBindings[sf::Keyboard::W] = [this]() { if (auto player = playerCharacter.lock()) player->addDirection(sf::Vector2i(0, -1) * reverseInputMultiplier); return sl::IS_MoveUp; };
	keyBindings[sf::Keyboard::A] = [this]() { if (auto player = playerCharacter.lock()) player->addDirection(sf::Vector2i(-1, 0) * reverseInputMultiplier); return sl::IS_MoveLeft; };
	keyBindings[sf::Keyboard::S] = [this]() { if (auto player = playerCharacter.lock()) player->addDirection(sf::Vector2i(0, 1) * reverseInputMultiplier); return sl::IS_MoveDown; };
	keyBindings[sf::Keyboard::D] = [this]() { if (auto player = playerCharacter.lock()) player->addDirection(sf::Vector2i(1, 0) * reverseInputMultiplier); return sl::IS_MoveRight; };
}
