#pragma once
#include <unordered_map>
#include <string>
#include <memory>

namespace sf {
	class Texture;
}

/*Необходимо перенести этот класс в FL_Client. FL_Server не должен иметь доступ к этому файлу*/
class TextureManager {
public:
	TextureManager();
	~TextureManager();
	std::shared_ptr<sf::Texture> getTexture(const std::string& path);
private:
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
};