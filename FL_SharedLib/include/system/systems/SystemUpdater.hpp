#pragma once
#include "SFML/System/Time.hpp"
#include "system/handlers/LockFreeDelegate.hpp"
#include <vector>
#include <memory>
#include <utility>


namespace sl {
	class IEntityRegistry;
	class CollisionSystem;
	class Entity;
	class UpdatableSystem;
	class CollisionCellMap;
	class WorldBase;

	//To establish new system dependencies in other projects, you need to create a structure that inherits from "SystemCreatorContext".
	struct SystemCreatorContext {
		SystemCreatorContext(const sl::CollisionCellMap& collisionCellMap, const sl::IEntityRegistry& entityRegistry):
			collisionCellMap(collisionCellMap), entityRegistry(entityRegistry){}
		const sl::CollisionCellMap& collisionCellMap;
		const sl::IEntityRegistry& entityRegistry;
	};

	enum class SystemProcessingPhase: uint8_t {
		Input = 1,
		Simulation = 2,
		PostSimulation = 3,
	};

	using systemPtr = std::unique_ptr<sl::UpdatableSystem>;

	using systemCreator = std::function<systemPtr(SystemCreatorContext)>;

	class SystemUpdater {
	public:

		SystemUpdater(const SystemCreatorContext& context);
		virtual ~SystemUpdater();

		void update(float deltaTime, const sl::WorldBase& world);

		static void registerSystem(systemCreator creator, SystemProcessingPhase processingPhase);

	protected:
		const sf::Time updateTime = sf::seconds(1.f / 60.f);
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		std::vector<std::unique_ptr<sl::UpdatableSystem>> inputSystems;
		std::vector<std::unique_ptr<sl::UpdatableSystem>> simulationSystems;
		std::vector<std::unique_ptr<sl::UpdatableSystem>> postSimulationSystems;

		std::unique_ptr<SystemCreatorContext> creatorContext;

		void createSystems();

		static std::vector<std::pair<systemCreator, SystemProcessingPhase>>& getRegistry() {
			static std::vector<std::pair<systemCreator, SystemProcessingPhase>> registry;
			return registry;
		}

		virtual void onUpdate(float deltaTime);

		virtual void onEnitiesUpdate(float deltaTime, const sl::Entity& entity);

	};

}