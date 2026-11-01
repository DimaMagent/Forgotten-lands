#include "pch.hpp"
#include "ServerDataLoader.hpp"
#include <fstream>
#include "system/EntityType.hpp"
#include "system/Utils.hpp"

std::vector<std::string> ServerDataLoader::filePaths = { "manifests/Characters.json" };

ServerDataLoader::ServerDataLoader()
{
	loadData();
}

void ServerDataLoader::loadData()
{
	try {
		for (const std::string& path : filePaths) {
			std::ifstream file(path);

			if (file.is_open()) {
				nlohmann::json data;
				file >> data;

				for (auto& [typeName, entityData] : data.items()) {
					jsonFileData[path].try_emplace(sl::hashEntityType(typeName), entityData);
				}

				file.close();
			}
			else {
				throw std::runtime_error("Could not open file with current path: " + path);
				return;
			}
		}
	}
	catch (std::exception& e) {
		std::shared_ptr<spdlog::logger> load_logger = spdlog::get("load");
		if (load_logger) {
			load_logger->error("ServerDataLoader::loadData exception: {}", e.what());
		}
		else {
			std::cerr << "load_logger is not valid. ServerDataLoader::loadData exception:" << e.what() << "\n";
		}
	}
}

std::optional<json> ServerDataLoader::getEntityData(sl::entityType type) {
	if (filePaths.empty()) { return {}; }

	auto enData = jsonFileData.find(filePaths[0]); //temp
	if (enData == jsonFileData.end()) { return {}; }

	auto jsonData = enData->second.find(type);
	if (jsonData == enData->second.end()) { return {}; }

	return jsonData->second;
}

std::optional<json> ServerDataLoader::getEntityData(std::string typeName)
{
	if (filePaths.empty()) { return {}; }

	auto enData = jsonFileData.find(filePaths[0]); //temp
	if (enData == jsonFileData.end()) { return {}; }

	auto jsonData = enData->second.find(sl::hashEntityType(typeName));
	if (jsonData == enData->second.end()) { return {}; }

	return jsonData->second;
	return std::optional<json>();
}
