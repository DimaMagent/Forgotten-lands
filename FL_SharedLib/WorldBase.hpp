#pragma once
#include <vector>
#include <memory>
#include "SFML/System/Time.hpp"

namespace sl {
	class Entity;
}

namespace sl {
	class WorldBase {
	public:
		WorldBase() = default;
		virtual ~WorldBase();
		void addEntity(std::unique_ptr<sl::Entity>&& entity);
		void update(float deltaTime);
		void removeEntity(size_t index);
	protected:
		std::vector<std::unique_ptr<sl::Entity>> Entities;
		const sf::Time updateTime = sf::seconds(1.f / 60.f);
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		virtual void onUpdate(float updateTime) = 0;
	};
}