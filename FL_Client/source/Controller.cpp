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
	try {
		if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
			if (keyEvent->scancode == sf::Keyboard::Scan::Unknown) return;

			switch (keyEvent->code) {
			case sf::Keyboard::Key::W: keyStates.at(sf::Keyboard::Key::W) = true; updateMovementDirection(); break;
			case sf::Keyboard::Key::A: keyStates.at(sf::Keyboard::Key::A) = true; updateMovementDirection(); break;
			case sf::Keyboard::Key::S: keyStates.at(sf::Keyboard::Key::S) = true; updateMovementDirection(); break;
			case sf::Keyboard::Key::D: keyStates.at(sf::Keyboard::Key::D) = true; updateMovementDirection(); break;
			case sf::Keyboard::Key::Q:
				onNewAction.broadcast(sl::net::Action::Attack);
				break;
			default: break;

			}
		}

		if (const auto* keyEvent = event.getIf<sf::Event::KeyReleased>()) {
			switch (keyEvent->code) {
			case sf::Keyboard::Key::W: keyStates.at(sf::Keyboard::Key::W) = false; updateMovementDirection(); break;
			case sf::Keyboard::Key::A: keyStates.at(sf::Keyboard::Key::A) = false; updateMovementDirection(); break;
			case sf::Keyboard::Key::S: keyStates.at(sf::Keyboard::Key::S) = false; updateMovementDirection(); break;
			case sf::Keyboard::Key::D: keyStates.at(sf::Keyboard::Key::D) = false; updateMovementDirection(); break;
			default: break;
			}
		}
	}
	catch (std::exception& e) {
		game_logger->warn("Controller::onEvent key bindings were not initialized. Starting initKeyBindings");
		initKeyBindings();
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

	keyStates.try_emplace(sf::Keyboard::Key::W, false);
	keyStates.try_emplace(sf::Keyboard::Key::A, false);
	keyStates.try_emplace(sf::Keyboard::Key::S, false);
	keyStates.try_emplace(sf::Keyboard::Key::D, false);
	keyStates.try_emplace(sf::Keyboard::Key::Q, false);
}

void Controller::updateMovementDirection() {
	try {
		sf::Vector2i newDirection(0, 0);

		if (keyStates.at(sf::Keyboard::Key::W)) newDirection.y -= 1;
		if (keyStates.at(sf::Keyboard::Key::S)) newDirection.y += 1;
		if (keyStates.at(sf::Keyboard::Key::A)) newDirection.x -= 1;
		if (keyStates.at(sf::Keyboard::Key::D)) newDirection.x += 1;

		if (newDirection != lastMovementDirectionIntent) {
			lastMovementDirectionIntent = newDirection;
			onSetMovementDirection.broadcast(lastMovementDirectionIntent);
		}
	}
	catch (std::exception& e) {
		game_logger->warn("Controller::updateMovementDirection key bindings were not initialized. Starting initKeyBindings");
		initKeyBindings();
	}
}