#include "pch.hpp"
#include "Controller.hpp"
#include "InputManager.hpp"
#include "Entity.hpp"
#include "Utils.hpp"

Controller::Controller(InputManager& im, sl::LockFreeDelegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntity)
{
	game_logger = spdlog::get("game");
	lastMovementDirectionIntent = sf::Vector2i(0, 0);
	im.onEvent.addFunction([this](const sf::Event& event) { onEvent(event); });
	onSetPlayerEntity.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) { onPlayerEntitySet(playerEntity); });
	initKeyBindings();
}

Controller::~Controller() = default;


void Controller::onEvent(const sf::Event& event) {
	if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
		if (auto it = keyBindings.find(keyEvent->code); it != keyBindings.end()) {
			reverseInputMultiplier = 1;
			it->second();

		}
		return;
	}

	if (const auto* keyEvent = event.getIf<sf::Event::KeyReleased>()) {
		if (auto it = keyBindings.find(keyEvent->code); it != keyBindings.end()) {
			reverseInputMultiplier = -1;
			it->second();

		}
		return;
	}
}

void Controller::onPlayerEntitySet(std::weak_ptr<sl::Entity> playerEntity)
{
	if (playerEntity.expired()) {
		game_logger->warn("Received expired player character reference.");
		return;
	}
	game_logger->info("Player character set in controller.");
	this->playerEntity = playerEntity;
}

void Controller::initKeyBindings(){
	keyBindings[sf::Keyboard::Key::W] = [this]() {
		lastMovementDirectionIntent = sl::inBounds(
			lastMovementDirectionIntent + sf::Vector2i(0, -1) * reverseInputMultiplier,
			sf::Vector2i(-1, -1),
			sf::Vector2i(1, 1)
		);
		onSetMovementDirection.broadcast(lastMovementDirectionIntent);
		};

	keyBindings[sf::Keyboard::Key::A] = [this]() {
		lastMovementDirectionIntent = sl::inBounds(
			lastMovementDirectionIntent + sf::Vector2i(-1, 0) * reverseInputMultiplier,
			sf::Vector2i(-1, -1),
			sf::Vector2i(1, 1)
		);
		onSetMovementDirection.broadcast(lastMovementDirectionIntent);
		};

	keyBindings[sf::Keyboard::Key::S] = [this]() {
		lastMovementDirectionIntent = sl::inBounds(
			lastMovementDirectionIntent + sf::Vector2i(0, 1) * reverseInputMultiplier,
			sf::Vector2i(-1, -1),
			sf::Vector2i(1, 1)
		);
		onSetMovementDirection.broadcast(lastMovementDirectionIntent);
		};

	keyBindings[sf::Keyboard::Key::D] = [this]() {
		lastMovementDirectionIntent = sl::inBounds(
			lastMovementDirectionIntent + sf::Vector2i(1, 0) * reverseInputMultiplier,
			sf::Vector2i(-1, -1),
			sf::Vector2i(1, 1)
		);
		onSetMovementDirection.broadcast(lastMovementDirectionIntent);
		};

	keyBindings[sf::Keyboard::Key::Q] = [this]() {
		onNewAction.broadcast(sl::net::Action::Attack);
		};

}