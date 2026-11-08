#pragma once
#include <vector>
#include <memory>
#include <pair>
#include <functional>
#include <span>
#include <concepts>
#include <type_traits>
#include "system/systems/events/SystemMetaData.hpp"
#include "system/storages/TypeList.hpp"

namespace sl {
	class UpdatableSystem;
	class CollisionGrid;
	class IEntityRegistry;

	template <typename T>
	concept SystemType = requires {
		typename T::RequiresComponents;

		typename T::RequiresResult;

		typename T::ProducesResult;

		requires sl::is_type_list_v<typename T::RequiresComponents>;
		
		requires sl::is_type_list_v<typename T::RequiresResult>;

		requires sl::is_type_list_v<typename T::ProducesResult>;

	};

	struct SystemCreatorContext {
		SystemCreatorContext(const sl::CollisionGrid& collisionCellMap, const sl::IEntityRegistry& entityRegistry) :
			collisionCellMap(collisionCellMap), entityRegistry(entityRegistry) {
		}
		const sl::CollisionGrid& collisionCellMap;
		const sl::IEntityRegistry& entityRegistry;
	};

	using systemPtr = std::unique_ptr<sl::UpdatableSystem>;

	using systemCreator = std::function<systemPtr(SystemCreatorContext&)>;

	class RegisterSystems {
	public:

		RegisterSystems(const SystemCreatorContext& context) {
			creatorContext = std::make_unique<SystemCreatorContext>(context);
		}

		virtual ~RegisterSystems() = default;

		template <SystemType Type>
		static void registerSystem(systemCreator creator) {
			sl::SystemMetaData metaData;

			metaData.components = sl::toTypeVector(typename Type::RequiresComponents);

			metaData.requires = sl::toTypeVector(typename Type::RequiresResult);

			metaData.produces = sl::toTypeVector(typename Type::ProducesResult);

			getRegistry().push_back(std::make_pair(std::move(creator), std::move(metaData)));
		}

		const std::span<std::pair<sl::systemCreator, sl::SystemMetaData> getRegisteredSystems() const { return getRegistry(); }

	private:

		std::unique_ptr<SystemCreatorContext> creatorContext;

		static std::vector<std::pair<systemCreator, sl::SystemMetaData>>& getRegistry() {
			static std::vector<std::pair<systemCreator, sl::SystemMetaData>> registry;
			return registry;
		}

	};

}