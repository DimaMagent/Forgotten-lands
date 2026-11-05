#pragma once
#include <vector>
#include <memory>
#include "SFML/System/Time.hpp"
#include <optional>
#include <functional>
#include <span>
#include "IEntityRegistry.hpp"
#include "IEntityStorage.hpp"
#include "EntityId.hpp"
#include "EntityType.hpp"
#include "system/handlers/LockFreeDelegate.hpp"

namespace sl {
	class Entity;
	class WorldGridsManager;
	class CollisionSystem;
	class SystemUpdater;
}

namespace sl {
	class WorldBase: public IEntityRegistry, public IEntityStorage {
	public:
		sl::LockFreeDelegate<sl::EntityId> OnEntityAdded;
		sl::LockFreeDelegate<sl::EntityId> OnEntityRemoved;

		WorldBase();
		virtual ~WorldBase();
		void update(float deltaTime);
		virtual sl::EntityId addEntity(sl::entityType entityType) = 0;
		virtual sl::EntityId addEntity(const std::string& entityTypeName) = 0;
		virtual void removeEntityById(sl::EntityId id) = 0;
		const std::optional<std::reference_wrapper<sl::WorldGridsManager>> getWorldMap() const;

	protected:
		const sf::Time updateTime = sf::seconds(1.f / 60.f);
		sf::Time timeSinceLastUpdate = sf::Time::Zero;
		
		virtual void onUpdate(float deltaTime);
	
	};
}