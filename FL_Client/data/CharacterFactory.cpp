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
	auto it = characterIdToDataId.find(characterId);
	if (it == characterIdToDataId.end()) {
		std::cout << "Character ID not found: " << static_cast<int>(characterId) << std::endl;
		return nullptr;
	}

	std::string dataId = it->second;
	JsonData jd = dataLoader->getData(dataId);

	if (auto renderCompIt = renderCompCache.find(characterId); renderCompIt != renderCompCache.end()) {
		return std::make_unique<Character>(renderCompIt->second, TransformComponent(), MovementComponent (jd.maxVelocity));
	}
	RenderComponent rc(textureManager->getTexture(jd.texturePath), jd.textureRect);
	renderCompCache.try_emplace(characterId, std::move(rc));

	return std::make_unique<Character>(renderCompCache.find(characterId)->second, TransformComponent(), MovementComponent(jd.maxVelocity));
}

void CharacterFactory::InitializeCharacterIdToDataId()
{
	characterIdToDataId.try_emplace(CharacterType::Player, "Player");
}

