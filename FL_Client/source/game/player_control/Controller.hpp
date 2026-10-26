#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "system/handlers/LockFreeDelegate.hpp"
#include "system/systems/Action.hpp"
#include "system/EntityId.hpp"

class InputManager;
namespace sl {
	class Entity;
}

class Controller {
public:
	Controller(InputManager& im, sl::LockFreeDelegate<std::optional<sl::EntityId>>& onSetPlayerEntity);

	~Controller();

	sl::LockFreeDelegate<sl::EntityId, sl::Action, bool> onNewAction;

	sl::LockFreeDelegate<sl::EntityId, const sf::Vector2i&> onSetMovementDirection;

private:
	std::shared_ptr<spdlog::logger> game_logger;

	std::unordered_map<sf::Keyboard::Key, bool> keyStates;

	std::optional<sl::EntityId> playerEntityId;

	sf::Vector2i lastMovementDirectionIntent;

	std::shared_ptr<sl::DelegateToken> token;

	void onEvent(const sf::Event& event);

	void initKeyBindings();

	void updateMovementDirection();

	void resetAllKeyStates();


};