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
	Controller(InputManager& im, sl::LockFreeDelegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntity);

	~Controller();

	sl::LockFreeDelegate<sl::net::Action> onNewAction;

	sl::LockFreeDelegate<const sf::Vector2i&> onSetMovementDirection;

private:
	std::shared_ptr<spdlog::logger> game_logger;

	std::unordered_map<sf::Keyboard::Key, bool> keyStates;

	std::weak_ptr<sl::Entity> playerEntity;

	sf::Vector2i lastMovementDirectionIntent;

	int reverseInputMultiplier = -1;

	void onEvent(const sf::Event& event);

	void onPlayerEntitySet(std::weak_ptr<sl::Entity> playerEntity);

	void initKeyBindings();

	void updateMovementDirection();


};