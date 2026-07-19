#include "pch.hpp"
#include "Controller.hpp"
#include "InputManager.hpp"
#include "LocalWorld.hpp"
#include "Entity.hpp"
#include "Packer.hpp"
#include "MovementComponent.hpp"
#include "InputStatePacket.hpp"
#include "Utils.hpp"

Controller::Controller(InputManager& im, LocalWorld& world)
{
	game_logger = spdlog::get("game") ;
	im.onEvent.addFunction([this](const sf::Event& event) { onEvent(event); });
	world.OnSetPlayerEntity.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) { onPlayerEntitySet(playerEntity); });
}

Controller::~Controller() = default;


void Controller::onEvent(const sf::Event& event) {
	if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
		if (auto it = keyBindings.find(keyEvent->code); it != keyBindings.end()) {
			reverseInputMultiplier = 1;
			isInputStateChanged = true;
			it->second();

		}
		return;
	}
	if (const auto* keyEvent = event.getIf<sf::Event::KeyReleased>()) {
		if (auto it = keyBindings.find(keyEvent->code); it != keyBindings.end()) {
			reverseInputMultiplier = -1;
			isInputStateChanged = true;
			it->second();

		}
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
	initKeyBindings();
}

void Controller::initKeyBindings(){
	keyBindings[sf::Keyboard::Key::W] = [this]() {
		currentInputStates.movementDirectionIntentions = sl::inBounds(currentInputStates.movementDirectionIntentions + sf::Vector2i(0, -1) * reverseInputMultiplier, sf::Vector2i(-1, -1), sf::Vector2i(1, 1));
		return sl::net::IS_MoveUp; };

	keyBindings[sf::Keyboard::Key::A] = [this]() {
		currentInputStates.movementDirectionIntentions = sl::inBounds(currentInputStates.movementDirectionIntentions + sf::Vector2i(-1, 0) * reverseInputMultiplier, sf::Vector2i(-1, -1), sf::Vector2i(1, 1));
		return sl::net::IS_MoveLeft; };

	keyBindings[sf::Keyboard::Key::S] = [this]() {
		currentInputStates.movementDirectionIntentions = sl::inBounds(currentInputStates.movementDirectionIntentions + sf::Vector2i(0, 1) * reverseInputMultiplier, sf::Vector2i(-1, -1), sf::Vector2i(1, 1));
		return sl::net::IS_MoveDown; };

	keyBindings[sf::Keyboard::Key::D] = [this]() {
		currentInputStates.movementDirectionIntentions = sl::inBounds(currentInputStates.movementDirectionIntentions + sf::Vector2i(1, 0) * reverseInputMultiplier, sf::Vector2i(-1, -1), sf::Vector2i(1, 1));
		return sl::net::IS_MoveRight; };
}

void Controller::setMovingDirection()
{
	auto player = playerEntity.lock();
	if (!player) { return; }

	sl::MovementComponent* movComp = player->getComponent<sl::MovementComponent>();

	if (!movComp) { return; }


	movComp->setVelocityDirection(currentInputStates.movementDirectionIntentions);

}

void Controller::tick(float dt) {
	timeSinceLastUpdate += std::min(sf::seconds(dt), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;
		if (!isInputStateChanged) { return; }
		
		Packer::send<sl::net::InputStatePacket>(currentInputStates.movementDirectionIntentions, 0, 0);

		setMovingDirection();

		isInputStateChanged = false;
	}
}