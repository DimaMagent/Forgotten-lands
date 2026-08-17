#pragma once
#include <vector>
#include <memory>
#include "SFML/System/Time.hpp"
#include "EntityStorage.hpp"
#include <optional>
#include <functional>

namespace sl {
	class Entity;
	class MovementSystem;
	class WorldMap;
	class CollisionSystem;
}

namespace sl {
	class WorldBase {
	public:
		WorldBase();
		virtual ~WorldBase();
		void update(float deltaTime);
		virtual void addEntity(std::unique_ptr<sl::Entity> entity, uint32_t id);
		virtual bool removeEntityById(uint32_t id);
		virtual std::optional<std::reference_wrapper<sl::Entity>> getEntityById(uint32_t id) const;
		virtual const EntityStorage& getEntityStorage() const { return entities; }
	protected:
		sl::EntityStorage entities;
		const sf::Time updateTime = sf::seconds(1.f / 60.f);
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		std::unique_ptr<sl::WorldMap> worldMap;

		std::unique_ptr<sl::CollisionSystem> collisionSystem;
		std::unique_ptr<MovementSystem> movementSystem;
		

		virtual void onUpdate(float updateTime) = 0;
		virtual void onUpdateEntities(sl::Entity& en, float updateTime) = 0;
	};
}