#pragma once
#include <SFML/System/Vector2.hpp>
#include "system/EntityId.hpp"
#include "system/systems/UpdatableSystem.hpp"

namespace sl {
	class Entity;
	class IEntityRegistry;
	class MovementComponent;

	class MovementSystem: public sl::UpdatableSystem {
	public:
		MovementSystem(const IEntityRegistry& entityRegistry);

		void update(float updateTime, const sl::Entity& entity) override;

	private:

		const IEntityRegistry& entityRegistry;

		void moving(float updateTime, const sl::Entity& entity, const IEntityRegistry& entityRegistry);

		sf::Vector2f calculateDelta(float deltaTime, sl::MovementComponent& movComp);

		void braking(float deltaTime, sl::MovementComponent& movComp);

		RegisterUpdatableSystem();
	};
}