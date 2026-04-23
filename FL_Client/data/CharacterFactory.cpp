#include "pch.hpp"
#include "CharacterFactory.hpp"
#include "Character.hpp"
#include "DataLoader.hpp"
#include "TextureManager.hpp"
#include "RenderComponent.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"

CharacterFactory::CharacterFactory() {
	dataLoader = std::make_unique<DataLoader>();
	textureManager = std::make_unique<TextureManager>();
	InitializeCharacterIdToDataId();
}

CharacterFactory::~CharacterFactory() = default;

std::unique_ptr<Character> CharacterFactory::createCharacter(const CharacterType characterId)
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

void CharacterFactory::InitializeCharacterIdToDataId()
{
	characterIdToDataId.try_emplace(CharacterType::Player, "Player");
}

