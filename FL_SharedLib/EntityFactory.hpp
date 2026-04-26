#pragma once
#include <unordered_map>
#include <memory>


class Entity;
class DataLoader;
class TextureManager;
class RenderComponent;

namespace sl {
	enum class EntityType {
		Player,
	};

	class EntityFactory {
	public:
		EntityFactory();
		~EntityFactory();
		std::unique_ptr<Entity> createEntity(const EntityType EntityId);
	private:
		std::unique_ptr<DataLoader> dataLoader;
		std::unique_ptr<TextureManager> textureManager;
		std::unordered_map<EntityType, std::shared_ptr<RenderComponent>> renderCompCache;
		std::unordered_map<EntityType, std::string> characterIdToDataId;

		void InitializeCharacterIdToDataId();
	};
}