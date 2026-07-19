#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "SFML/System/Time.hpp"
#include "PacketDataTypes.hpp"

class InputManager;
class LocalWorld;
namespace sl {
	class Entity;
}

struct Intentions {
	sf::Vector2i movementDirectionIntentions;
	uint8_t inputState;
};



class Controller {
public:
	Controller(InputManager& im, LocalWorld& world);
	~Controller();
	void tick(float dt);
private:
	std::shared_ptr<spdlog::logger> game_logger;

	std::unordered_map<sf::Keyboard::Key, std::function<sl::net::InputState()>> keyBindings;
	std::weak_ptr<sl::Entity> playerEntity;
	int reverseInputMultiplier = -1;
	bool isInputStateChanged = false;
	Intentions currentInputStates;
	const sf::Time updateTime = sf::seconds(1.f / 30.f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	void onEvent(const sf::Event& event);
	void onPlayerEntitySet(std::weak_ptr<sl::Entity> playerEntity);
	void initKeyBindings();
	void setMovingDirection();
};