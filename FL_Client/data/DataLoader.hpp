#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <json.hpp>

struct JsonData{
	std::string texturePath;
	sf::IntRect textureRect;
	float maxVelocity;
};


class DataLoader {
public:
	DataLoader();
	DataLoader(const DataLoader&) = delete;
	~DataLoader();
	JsonData getData(const std::string& id);
private:
	void fromJson(const nlohmann::json& json, const std::string& id, JsonData& out);
};