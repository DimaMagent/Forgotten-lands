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
