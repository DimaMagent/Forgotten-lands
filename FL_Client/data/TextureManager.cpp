#include "TextureManager.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager() = default;

const sf::Texture& TextureManager::getTexture(const std::string & path)
{
	auto it = textures.find(path);
	if (it != textures.end()) {
		return it->second;
	}
	sf::Texture texture;
	if (!texture.loadFromFile(path)) {
		std::cout << "Failed to load texture: " << path << std::endl;
		throw std::runtime_error("Failed to load texture: " + path);
	}

	textures[path] = std::move(texture);
	return textures[path];
}
