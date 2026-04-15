#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>


class Character;
class DataLoader;
class TextureManager;
class RenderComponent;

enum class CharacterType {
	Player,
};

class CharacterFactory {
public:
	CharacterFactory();
	~CharacterFactory();
	std::unique_ptr<Character> createCharacter(const CharacterType characterId);
private:
	std::unique_ptr<DataLoader> dataLoader;
	std::unique_ptr<TextureManager> textureManager;
	std::unordered_map<CharacterType, RenderComponent> renderCompCache;
	std::unordered_map<CharacterType, std::string> characterIdToDataId;

	void InitializeCharacterIdToDataId();
};