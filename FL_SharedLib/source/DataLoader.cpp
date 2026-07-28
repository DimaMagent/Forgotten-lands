#include "pch.h"
#include "DataLoader.hpp"
#include <fstream>

sl::DataLoader::DataLoader(){}

sl::DataLoader::~DataLoader() = default;


json sl::DataLoader::getData(const std::string& id, const std::string& pathToFile)
{
	try {
		std::ifstream file(pathToFile);
		if (file.is_open()) {
			nlohmann::json data;
			file >> data;
			auto& EntityData = data.at(id);
			return std::move(EntityData);
		}
		else {
			throw std::runtime_error("Could not open file with current path");
		}

	}
	catch (std::exception& e) {
		std::cerr << "DataLoader::getData exception: " << e.what() << "\n";
	}
}
