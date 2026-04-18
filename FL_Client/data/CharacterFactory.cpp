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

Character CharacterFactory::createCharacter(const CharacterType characterId)
{
	try {
		auto it = characterIdToDataId.find(characterId);
		if (it == characterIdToDataId.end()) {
			throw std::runtime_error("Character ID not found: " + std::to_string(static_cast<int>(characterId)));
		}

		std::string dataId = it->second;
		JsonData jd = dataLoader->getData(dataId);

		if (auto renderCompIt = renderCompCache.find(characterId); renderCompIt != renderCompCache.end()) {
			return Character(renderCompIt->second, TransformComponent(), MovementComponent(jd.maxVelocity));
		}
		RenderComponent rc(textureManager->getTexture(jd.texturePath), jd.textureRect);
		renderCompCache.try_emplace(characterId, std::move(rc));

		return Character(renderCompCache.find(characterId)->second, TransformComponent(), MovementComponent(jd.maxVelocity));
	}
	catch (const std::exception& e) {
		std::cerr << "Error creating character: " << e.what() << std::endl;
		return Character();
	}
}

void CharacterFactory::InitializeCharacterIdToDataId()
{
	characterIdToDataId.try_emplace(CharacterType::Player, "Player");
}

