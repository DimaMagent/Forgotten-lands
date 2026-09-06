#pragma once
#include <vector>
#include <memory>
#include "SFML/System/Time.hpp"
#include <optional>
#include <functional>
#include <span>
#include "IEntityRegistry.hpp"
#include "EntityId.hpp"

namespace sl {
	class Entity;
	class MovementSystem;
	class WorldMap;
	class CollisionSystem;
	class SystemUpdater;
}

namespace sl {
	class WorldBase: public IEntityRegistry {
	public:
		WorldBase(std::unique_ptr<sl::SystemUpdater> currentSystemUpdater);
		virtual ~WorldBase();
		virtual void update(float deltaTime);
		virtual void removeEntityById(sl::EntityId id) = 0;
		virtual std::span<sl::Entity> getEntities() = 0;
		virtual std::span<const sl::Entity> getEntities() const = 0;
		const std::optional<std::reference_wrapper<sl::WorldMap>> getWorldMap() const;

	protected:
		const sf::Time updateTime = sf::seconds(1.f / 60.f);
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		std::unique_ptr<sl::WorldMap> worldMap;
		std::unique_ptr<sl::SystemUpdater> systemUpdater;
	
	};
}