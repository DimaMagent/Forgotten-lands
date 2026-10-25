#include "pch.hpp"
#include "Controller.hpp"
#include "InputManager.hpp"
#include "system/Entity.hpp"
#include "system/Utils.hpp"

Controller::Controller(InputManager& im, sl::LockFreeDelegate<std::optional<sl::EntityId>>& onSetPlayerEntity)
{
	game_logger = spdlog::get("game");
	lastMovementDirectionIntent = sf::Vector2i(0, 0);
	token = im.onEvent.addFunction([this](const sf::Event& event) { onEvent(event); });
	initKeyBindings();
	token = onSetPlayerEntity.addFunction([this](std::optional<sl::EntityId> playerEntityId) {
		this->playerEntityId = playerEntityId;

		if (playerEntityId.has_value()) {
			game_logger->info("Player with id: {} character set in Controller.", playerEntityId.value().ID);
		}

		});
}

Controller::~Controller() = default;


void Controller::onEvent(const sf::Event& event) {
	if (const auto* keyEvent = event.getIf<sf::Event::FocusLost>()) {
		resetAllKeyStates();
	}

	if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
		if (keyEvent->scancode == sf::Keyboard::Scan::Unknown) return;

		switch (keyEvent->code) {
		case sf::Keyboard::Key::W: keyStates.at(sf::Keyboard::Key::W) = true; updateMovementDirection(); break;
		case sf::Keyboard::Key::A: keyStates.at(sf::Keyboard::Key::A) = true; updateMovementDirection(); break;
		case sf::Keyboard::Key::S: keyStates.at(sf::Keyboard::Key::S) = true; updateMovementDirection(); break;
		case sf::Keyboard::Key::D: keyStates.at(sf::Keyboard::Key::D) = true; updateMovementDirection(); break;
		case sf::Keyboard::Key::Q:
			if (playerEntityId.has_value()) {
				onNewAction.broadcast(playerEntityId.value(), sl::Action::Attack);
			}
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
		case sf::Keyboard::Key::Q:
			if (playerEntityId.has_value()) {
				onNewAction.broadcast(playerEntityId.value(), sl::Action::None);
			}
			break;
		default: break;
		}
	}

	if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
		//std::cout << "Mouse position: (" << mouseMoved->position.x << ", " << mouseMoved->position.y << ")" << std::endl;
	}
}

void Controller::initKeyBindings(){

	keyStates.try_emplace(sf::Keyboard::Key::W, false);
	keyStates.try_emplace(sf::Keyboard::Key::A, false);
	keyStates.try_emplace(sf::Keyboard::Key::S, false);
	keyStates.try_emplace(sf::Keyboard::Key::D, false);
	keyStates.try_emplace(sf::Keyboard::Key::Q, false);
}

void Controller::updateMovementDirection() {
	sf::Vector2i newDirection(0, 0);

	if (keyStates.at(sf::Keyboard::Key::W)) newDirection.y -= 1;
	if (keyStates.at(sf::Keyboard::Key::S)) newDirection.y += 1;
	if (keyStates.at(sf::Keyboard::Key::A)) newDirection.x -= 1;
	if (keyStates.at(sf::Keyboard::Key::D)) newDirection.x += 1;

	if (newDirection != lastMovementDirectionIntent) {
		if (playerEntityId.has_value()) {
			lastMovementDirectionIntent = newDirection;
			onSetMovementDirection.broadcast(playerEntityId.value(), lastMovementDirectionIntent);
		}
	}
}

void Controller::resetAllKeyStates()
{
	for (auto it = keyStates.begin(); it != keyStates.end(); ++it) {
		it->second = false;
	}
	updateMovementDirection();
}
