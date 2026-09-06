#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "LockFreeDelegate.hpp"
#include "PacketDataTypes.hpp"

class InputManager;
namespace sl {
	class Entity;
}

class Controller {
public:
	Controller(InputManager& im);

	~Controller();

	sl::LockFreeDelegate<sl::net::Action> onNewAction;

	sl::LockFreeDelegate<const sf::Vector2i&> onSetMovementDirection;

private:
	std::shared_ptr<spdlog::logger> game_logger;

	std::unordered_map<sf::Keyboard::Key, bool> keyStates;

	std::weak_ptr<sl::Entity> playerEntity;

	sf::Vector2i lastMovementDirectionIntent;

	void onEvent(const sf::Event& event);

	void initKeyBindings();

	void updateMovementDirection();

	void resetAllKeyStates();


};