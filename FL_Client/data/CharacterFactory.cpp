#include "CharacterFactory.hpp"
#include "Character.hpp"
#include "DataLoader.hpp"
#include "TextureManager.hpp"
#include "RenderComponent.hpp"

CharacterFactory::CharacterFactory() {
	dataLoader = std::make_unique<DataLoader>();
	textureManager = std::make_unique<TextureManager>();
	InitializeCharacterIdToDataId();
}

CharacterFactory::~CharacterFactory() = default;

std::unique_ptr<Character> CharacterFactory::createCharacter(const CharacterType characterId)
{
	if (auto renderCompIt = renderCompCache.find(characterId); renderCompIt != renderCompCache.end()) {
		return std::make_unique<Character>(renderCompIt->second);
	}
	auto it = characterIdToDataId.find(characterId);
	if (it == characterIdToDataId.end()) {
		std::cout << "Character ID not found: " << static_cast<int>(characterId) << std::endl;
		return nullptr;
	}
	std::string dataId = it->second;
	RenderData rd = dataLoader->getData(dataId);
	RenderComponent rc(textureManager->getTexture(rd.texturePath), rd.textureRect);
	renderCompCache.try_emplace(characterId, std::move(rc));
	std::unique_ptr<Character> character = std::make_unique<Character>(renderCompCache.find(characterId)->second);
	return character;
}

void CharacterFactory::InitializeCharacterIdToDataId()
{
	characterIdToDataId.try_emplace(CharacterType::Player, "Player");
}

