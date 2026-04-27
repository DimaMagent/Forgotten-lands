#include "pch.h"
#include "DataLoader.hpp"
#include <fstream>

DataLoader::DataLoader(){}

DataLoader::~DataLoader() = default;


json& DataLoader::getData(const std::string& id)
{
	std::ifstream file("resources/Characters.json");
	if (!file.is_open()) {
		throw std::runtime_error("Could not open Characters.json");
	}
	nlohmann::json data;
	file >> data;
	auto& EntityData = data.at(id);
	return EntityData;
}

//
//void DataLoader::fromJson(const nlohmann::json& json, const std::string& id, JsonData& out)
//{
//	try {
//		
//		characterData.at("maxVelocity").get_to(out.maxVelocity);
//		characterData.at("texturePath").get_to(out.texturePath);
//		auto& rect = characterData.at("textureRect");
//		rect.at("x").get_to(out.textureRect.left);
//		rect.at("y").get_to(out.textureRect.top);
//		rect.at("width").get_to(out.textureRect.width);
//		rect.at("height").get_to(out.textureRect.height);
//	}
//	catch (const nlohmann::json::exception& e) {
//		std::cerr << "JSON error: " << e.what() << std::endl;
//		throw;
//	}
//}
