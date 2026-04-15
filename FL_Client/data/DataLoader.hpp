#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <json.hpp>

struct RenderData{
	std::string texturePath;
	sf::IntRect textureRect;
};


class DataLoader {
public:
	DataLoader();
	DataLoader(const DataLoader&) = delete;
	~DataLoader();
	RenderData getData(const std::string& id);
private:
	void fromJson(const nlohmann::json& json, const std::string& id, RenderData& out);
};