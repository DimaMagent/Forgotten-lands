#pragma once
#include "SFML/System/Time.hpp"
#include "system/handlers/LockFreeDelegate.hpp"
#include <vector>
#include <memory>


namespace sl {
	class WorldBase;
	class CollisionSystem;
	class Entity;
	class UpdatableSystem;
	class CollisionCellMap;


	struct SystemCreatorContext {
		SystemCreatorContext(const sl::CollisionCellMap& collisionCellMap, const WorldBase& worldBase):
			collisionCellMap(collisionCellMap), worldBase(worldBase){}
		const sl::CollisionCellMap& collisionCellMap;
		const WorldBase& worldBase;
	};

	using systemPtr = std::unique_ptr<sl::UpdatableSystem>;

	using systemCreator = std::function<systemPtr(SystemCreatorContext)>;

	class SystemUpdater {
	public:

		SystemUpdater(const SystemCreatorContext& context);
		virtual ~SystemUpdater();

		void update(float deltaTime, const sl::WorldBase& world);

		static void registerSystem(systemCreator creator);

	protected:
		const sf::Time updateTime = sf::seconds(1.f / 60.f);
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		std::unique_ptr<sl::CollisionSystem> collisionSystem;

		std::vector<std::unique_ptr<sl::UpdatableSystem>> systems;

		std::unique_ptr<SystemCreatorContext> creatorContext;

		void createSystems();

		static std::vector<systemCreator>& getRegistry() {
			static std::vector<systemCreator> registry;
			return registry;
		}

		virtual void onUpdate(float deltaTime, const sl::WorldBase& world);

		virtual void onEnitiesUpdate(float deltaTime, const sl::WorldBase& world, const sl::Entity& entity);

	};

}