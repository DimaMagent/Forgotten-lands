#include "pch.hpp"
#include "game/render/CameraComponent.hpp"
#include "SFML/Graphics/View.hpp"
#include "system/Entity.hpp"

CameraComponent::CameraComponent(sf::Vector2f cameraPosition, sf::Vector2f cameraSize) {
	camera = std::make_unique<sf::View>(cameraPosition, cameraSize);
}

void CameraComponent::setCameraPosition(sf::Vector2f newPos)
{
	if (!camera) {
		spdlog::error("CameraComponent::setCameraPosition camera is not valid");
		throw std::runtime_error("CameraComponent::setCameraPosition camera is not valid");
	}
	camera->setCenter(newPos);
}

void CameraComponent::setCameraSize(sf::Vector2f newSize)
{
	if (!camera) {
		spdlog::error("CameraComponent::setCameraSize camera is not valid");
		throw std::runtime_error("CameraComponent::setCameraSize camera is not valid");
	}
	camera->setSize(newSize);
}

void CameraComponent::initialize(sl::ComponentInitContext context) {
	if (!context.js) {
		context.entity.addComponent<CameraComponent>();
		return;
	}

	context.entity.addComponent<CameraComponent>(sf::Vector2f(
		context.js->value("centerPositionX", 0),
		context.js->value("centerPositionY", 0)),
		sf::Vector2f(context.js->value("cameraSizeX", 0),
		context.js->value("cameraSizeY", 0)));
}

const sf::View& CameraComponent::getCamera() {
	if (!camera) {
		spdlog::error("CameraComponent::getCamera camera is not valid");
		throw std::runtime_error("CameraComponent::getCamera camera is not valid");
	}
	return *camera;
}