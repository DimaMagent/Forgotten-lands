#pragma once
#include "system/systems/UpdatableSystem.hpp"

namespace sl {
	class Entity;
}

namespace sf {
	class RenderTarget;
}

class CameraMovementSystem: public sl::UpdatableSystem {
public:
	CameraMovementSystem(const sl::IEntityRegistry& entityRegistry, sf::RenderTarget& renderTarget);

	void update(float updateTime, const sl::Entity& entity);

private:
	const sl::IEntityRegistry& entityRegistry;

	sf::RenderTarget& renderTarget;

	RegisterUpdatableSystem();
};