#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include <string_view>
#include <nlohmann/json.hpp>
#include <any>
#include "EntityType.hpp"
#include "ComponentTypes.hpp"

/*If the compilation time due to nlohmann/json.hpp header is significant,
then PIMPL + component registration composition can be used to remove the inclusion from the .hpp file.
However, such measures are currently redundant.*/


class TextureManager;

using json = nlohmann::json;

namespace sl {
	class DataLoader;

	class Entity;

	struct ComponentInitContext {
		sl::Entity& entity;
		const json* js;
		std::any serviceProvider;

		template<typename T>
		T* getService() const {
			if (!serviceProvider.has_value()) return nullptr;

			if (auto* const* ptr = std::any_cast<T*>(&serviceProvider)) {
				return *ptr;
			}
			return nullptr;
		}
	};

	using ComponentFactory = std::function<void(ComponentInitContext context)>;

	class EntityFactory {
	public:
		EntityFactory(std::unique_ptr<sl::DataLoader>&& dataLoader);
		EntityFactory(EntityFactory&&) = default;
		virtual ~EntityFactory();

		std::unique_ptr<sl::Entity> createEntity(sl::EntityType entityType);

		static void registerComponent(sl::TypeID componentTypeId, sl::ComponentFactory factory);

	protected:

		virtual std::any getServiceProvider() { return {}; }

		static auto& getRegistry() {
			static std::unordered_map<sl::TypeID, ComponentFactory> registry;
			return registry;
		}

		std::unique_ptr<sl::DataLoader> dataLoader;
	};
}