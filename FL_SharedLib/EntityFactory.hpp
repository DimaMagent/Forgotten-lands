#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include <nlohmann/json.hpp>

/*If the compilation time due to nlohmann/json.hpp header is significant,
then PIMPL + component registration composition can be used to remove the inclusion from the .hpp file.
However, such measures are currently redundant.*/

class DataLoader;
class TextureManager;

namespace sl {
	class Entity;

	enum class EntityType {
		Player,
	};

	using json = nlohmann::json;

	using ComponentFactory = std::function<void(sl::Entity& entity, const json& js)>;

	//before using you should use methos initialize for currect work
	class EntityFactory {
	public:
		EntityFactory();
		EntityFactory(EntityFactory&&) = default;
		virtual ~EntityFactory();

		void initialize();

		std::unique_ptr<sl::Entity> createEntity(const EntityType entityId);
	protected:
		std::unique_ptr<DataLoader> dataLoader;
		std::unordered_map<sl::EntityType, std::string> characterIdToDataId;
		std::unordered_map<std::string, ComponentFactory> registry;

		void InitializeCharacterIdToDataId();
		virtual void registrationComponents();
	};
}