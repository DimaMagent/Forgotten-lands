#pragma once
#include <unordered_map>
#include <string>
#include <memory>

namespace sf {
	class Texture;
}

class TextureManager {
public:
	TextureManager();
	~TextureManager();
	std::shared_ptr<sf::Texture> getTexture(const std::string& path);
private:
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
};