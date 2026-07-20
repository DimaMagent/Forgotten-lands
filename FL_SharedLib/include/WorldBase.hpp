#pragma once
#include <vector>
#include <memory>
#include "SFML/System/Time.hpp"
#include "EntityStorage.hpp"

namespace sl {
	class Entity;
	class MovementSystem;
}

namespace sl {
	class WorldBase {
	public:
		WorldBase();
		virtual ~WorldBase();
		void addEntity(std::unique_ptr<sl::Entity>&& entity, uint32_t id);
		void update(float deltaTime);
		void removeEntity(size_t index);
	protected:
		sl::EntityStorage entities;
		const sf::Time updateTime = sf::seconds(1.f / 60.f);
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		std::unique_ptr<MovementSystem> movementSystem;

		virtual void onUpdate(float updateTime) = 0;
	};
}