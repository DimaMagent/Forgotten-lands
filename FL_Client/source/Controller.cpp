#include "pch.hpp"
#include "Controller.hpp"
#include "InputManager.hpp"
#include "LocalWorld.hpp"
#include "Entity.hpp"
#include "Packer.hpp"
#include "MovementComponent.hpp"

Controller::Controller(InputManager& im, LocalWorld& world)
{
	im.onEvent.addFunction([this](const sf::Event& event) { onEvent(event); });
	world.OnSetPlayerEntity.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) { onPlayerEntitySet(playerEntity); });
}

Controller::~Controller() = default;


void Controller::onEvent(const sf::Event& event) {
	if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
		if (auto it = keyBindings.find(keyEvent->code); it != keyBindings.end()) {
			std::cout << "Executed action for key: " << (int)keyEvent->code << std::endl;
			reverseInputMultiplier = 1;
			Packer::packageInputStatePacket(it->second(), true);
		}
		return;
	}
	if (const auto* keyEvent = event.getIf<sf::Event::KeyReleased>()) {
		if (auto it = keyBindings.find(keyEvent->code); it != keyBindings.end()) {
			std::cout << "Released action for key: " << (int)keyEvent->code << std::endl;
			reverseInputMultiplier = -1;
			Packer::packageInputStatePacket(it->second(), false);
		}
	}
}

void Controller::onPlayerEntitySet(std::weak_ptr<sl::Entity> playerEntity)
{
	if (playerEntity.expired()) {
		std::cout << "Received expired player character reference." << std::endl;
		return;
	}
	std::cout << "Player character set in controller." << std::endl;
	this->playerEntity = playerEntity;
	initKeyBindings();
}

void Controller::initKeyBindings(){
	keyBindings[sf::Keyboard::Key::W] = [this]() {
		moveEntity(sf::Vector2i(0, -1));
		return sl::IS_MoveUp; };

	keyBindings[sf::Keyboard::Key::A] = [this]() {
		moveEntity(sf::Vector2i(-1, 0));
		return sl::IS_MoveLeft; };

	keyBindings[sf::Keyboard::Key::S] = [this]() {
		moveEntity(sf::Vector2i(0, 1));
		return sl::IS_MoveDown; };

	keyBindings[sf::Keyboard::Key::D] = [this]() {
		moveEntity(sf::Vector2i(1, 0));
		return sl::IS_MoveRight; };
}

void Controller::moveEntity(sf::Vector2i direction)
{
	auto player = playerEntity.lock();
	if (!player) { return; }

	sl::MovementComponent* movComp = player->getComponent<sl::MovementComponent>();

	if (!movComp) { return; }

	movComp->addDirection(direction * reverseInputMultiplier);

}
