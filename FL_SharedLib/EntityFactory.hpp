#pragma once
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp> // TODO: как-то убрать это включение из hpp файла
#include <functional>
#include <string>



class DataLoader;
class TextureManager;
class RenderComponent;


namespace sl {
	class Entity;

	enum class EntityType {
		Player,
	};

	using json = nlohmann::json;

	using ComponentFactory = std::function<void(sl::Entity& entity, const json& js)>;

	class EntityFactory {
	public:
		EntityFactory();
		~EntityFactory();
		std::unique_ptr<sl::Entity> createEntity(const EntityType EntityId);
	protected:
		std::unique_ptr<DataLoader> dataLoader;
		std::unique_ptr<TextureManager> textureManager;
		std::unordered_map<EntityType, std::string> characterIdToDataId;
		std::unordered_map<std::string, ComponentFactory> registry;
	
		void InitializeCharacterIdToDataId();
		virtual void registrationComponents();
	};
}