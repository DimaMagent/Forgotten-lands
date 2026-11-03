#pragma once
#include "system/systems/Component.hpp"
#include <memory>
#include <optional>
#include "system/EntityId.hpp"

namespace sf {
	class View;
}

class CameraComponent : public sl::Component {
public:
	CameraComponent(sf::Vector2f cameraPosition = sf::Vector2f(0.0f, 0.0f), sf::Vector2f cameraSize = sf::Vector2f(0.0f, 0.0f));

	void setCameraPosition(sf::Vector2f newPos);

	void setCameraSize(sf::Vector2f newSize);
	
	const sf::View& getCamera();

	void setTargetEntity(sl::EntityId newTargetEntity) { targetEntityId = newTargetEntity; }

	std::optional<sl::EntityId> getTargetEntityId() const { return targetEntityId; }

	COMPONENT_TYPE(CameraComponent);
private:
	std::unique_ptr<sf::View> camera;

	std::optional<sl::EntityId> targetEntityId;

	REGISTER_COMPONENT();
};
