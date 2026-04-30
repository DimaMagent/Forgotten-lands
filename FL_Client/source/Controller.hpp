#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "PacketDataTypes.hpp"

class InputManager;
class LocalWorld;
namespace sl {
	class Entity;
}

class Controller {
public:
	Controller(InputManager& im, LocalWorld& world);
	~Controller();
private:
	std::unordered_map<sf::Keyboard::Key, std::function<sl::InputState()>> keyBindings;
	std::weak_ptr<sl::Entity> playerEntity;
	int reverseInputMultiplier = -1;

	void onEvent(const sf::Event& event);
	void onPlayerEntitySet(std::weak_ptr<sl::Entity> playerEntity);
	void initKeyBindings();
	void moveEntity(sf::Vector2i direction);
};