#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include "PacketDataTypes.hpp"

class InputManager;
class World;
class Character;
namespace sf {
	class Event;
}

class Controller {
public:
	Controller(InputManager& im, World& world);
private:
	std::unordered_map<sf::Keyboard::Key, std::function<sl::InputState()>> keyBindings;
	std::weak_ptr<Character> playerCharacter;
	int reverseInputMultiplier = -1;

	void onEvent(const sf::Event& event);
	void OnPlayerCharacterSet(std::weak_ptr<Character> playerCharacter);
	void InitKeyBindings();
};