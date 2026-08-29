#pragma once
#include <vector>
#include <memory>
#include "SFML/System/Time.hpp"
#include <optional>
#include <functional>
#include "EntityId.hpp"

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
		virtual void removeEntityById(sl::EntityId id) = 0;
		virtual std::optional<std::reference_wrapper<const sl::Entity>> getEntityById(sl::EntityId id) const = 0;
		virtual std::optional<std::reference_wrapper<sl::Entity>> getEntityById(sl::EntityId id) = 0;
		virtual const std::vector<sl::Entity>& getEntities() const = 0;
		const std::optional<std::reference_wrapper<sl::WorldMap>> getWorldMap() const;

	protected:
		const sf::Time updateTime = sf::seconds(1.f / 60.f);
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		std::unique_ptr<sl::WorldMap> worldMap;

		std::unique_ptr<sl::CollisionSystem> collisionSystem;
		std::unique_ptr<MovementSystem> movementSystem;
		
		virtual void onUpdate(float updateTime) = 0;
		virtual void onUpdateEntities(sl::Entity& en, float updateTime) = 0;
	};
}