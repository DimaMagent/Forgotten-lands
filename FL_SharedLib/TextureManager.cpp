#include "pch.h"
#include "TextureManager.hpp"
#include <SFML/Graphics.hpp>

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager() = default;

std::shared_ptr<sf::Texture> TextureManager::getTexture(const std::string & path)
{
	auto it = textures.find(path);
	if (it != textures.end()) {
		return it->second;
	}

	auto texturePtr = std::make_shared<sf::Texture>();
	if (!texturePtr->loadFromFile(path)) {
		std::cout << "Failed to load texture: " << path << std::endl;
		throw std::runtime_error("Failed to load texture: " + path);
	}

	textures[path] = texturePtr;
	return texturePtr;
}
