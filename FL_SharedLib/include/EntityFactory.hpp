#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include <string_view>
#include <nlohmann/json.hpp>
#include "EntityType.hpp"

/*If the compilation time due to nlohmann/json.hpp header is significant,
then PIMPL + component registration composition can be used to remove the inclusion from the .hpp file.
However, such measures are currently redundant.*/


class TextureManager;

using json = nlohmann::json;

namespace sl {
	class DataLoader;

	class Entity;

	using ComponentFactory = std::function<void(sl::Entity& entity, const json& js)>;

	//before using you should use methos initialize for currect work
	class EntityFactory {
	public:
		EntityFactory(std::unique_ptr<sl::DataLoader>&& dataLoader);
		EntityFactory(EntityFactory&&) = default;
		virtual ~EntityFactory();

		void initialize();

		std::unique_ptr<sl::Entity> createEntity(sl::EntityType entityType);

	protected:

		std::unique_ptr<sl::DataLoader> dataLoader;
		std::unordered_map<std::string_view, ComponentFactory> registry;

		virtual void registrationComponents() = 0;
	};
}