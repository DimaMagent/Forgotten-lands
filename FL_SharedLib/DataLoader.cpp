#include "pch.h"
#include "DataLoader.hpp"
#include <fstream>

DataLoader::DataLoader(){}

DataLoader::~DataLoader() = default;


json DataLoader::getData(const std::string& id)
{
	try {
		std::ifstream file("resources/Characters.json");
		if (file.is_open()) {
			nlohmann::json data;
			file >> data;
			auto& EntityData = data.at(id);
			return std::move(EntityData);
		}
		else {
			throw std::runtime_error("Could not open Characters.json");
		}

	}
	catch (std::exception& e) {
		std::cerr << "DataLoader::getData exception: " << e.what() << "\n";
	}
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
