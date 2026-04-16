#include "DataLoader.hpp"
#include <fstream>
#include <iostream>

DataLoader::DataLoader(){}

DataLoader::~DataLoader() = default;


JsonData DataLoader::getData(const std::string& id)
{
	std::ifstream file("resources/Characters.json");
	if (!file.is_open()) {
		throw std::runtime_error("Could not open Characters.json");
	}
	nlohmann::json data;
	file >> data;
	JsonData rd;
	fromJson(data, id, rd);
	return rd;
}

void DataLoader::fromJson(const nlohmann::json& json, const std::string& id, JsonData& out)
{
	try {
		auto& characterData = json.at(id);
		characterData.at("maxVelocity").get_to(out.maxVelocity);
		characterData.at("texturePath").get_to(out.texturePath);
		auto& rect = characterData.at("textureRect");
		rect.at("x").get_to(out.textureRect.left);
		rect.at("y").get_to(out.textureRect.top);
		rect.at("width").get_to(out.textureRect.width);
		rect.at("height").get_to(out.textureRect.height);
	}
	catch (const nlohmann::json::exception& e) {
		std::cerr << "JSON error: " << e.what() << std::endl;
		throw;
	}
}
