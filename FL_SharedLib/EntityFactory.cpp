#include "pch.h"
#include "EntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include "TextureManager.hpp"
#include "RenderComponent.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"

sl::EntityFactory::EntityFactory() {
	dataLoader = std::make_unique<DataLoader>();
	textureManager = std::make_unique<TextureManager>();
	InitializeCharacterIdToDataId();
}

sl::EntityFactory::~EntityFactory() = default;

std::unique_ptr<Entity> sl::EntityFactory::createEntity(const EntityType characterId)
{
	try {
		auto it = characterIdToDataId.find(characterId);
		if (it == characterIdToDataId.end()) {
			throw std::runtime_error("Character ID not found: " + std::to_string(static_cast<int>(characterId)));
		}

		std::string dataId = it->second;
		JsonData jd = dataLoader->getData(dataId);

		if (auto renderCompIt = renderCompCache.find(characterId); renderCompIt != renderCompCache.end()) {
			return std::make_unique<Character>(*renderCompIt->second, TransformComponent(), MovementComponent(jd.maxVelocity));
		}

		std::shared_ptr<RenderComponent> rcPtr = std::make_shared<RenderComponent>(textureManager->getTexture(jd.texturePath), jd.textureRect);
		renderCompCache.try_emplace(characterId, rcPtr);

		return std::make_unique<Character>(*renderCompCache.find(characterId)->second, TransformComponent(), MovementComponent(jd.maxVelocity));
	}
	catch (const std::exception& e) {
		std::cerr << "Error creating character: " << e.what() << std::endl;
		return nullptr;
	}
}

void sl::EntityFactory::InitializeCharacterIdToDataId()
{
	characterIdToDataId.try_emplace(EntityType::Player, "Player");
}

