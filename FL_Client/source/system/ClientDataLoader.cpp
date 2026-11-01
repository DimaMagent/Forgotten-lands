#include "pch.hpp"
#include "ClientDataLoader.hpp"
#include <fstream>
#include "system/EntityType.hpp"
#include "system/Utils.hpp"

std::vector<std::string> ClientDataLoader::filePaths = { "manifests/ClientCharacterComponents.json" };

ClientDataLoader::ClientDataLoader()
{
	loadData();
}

void ClientDataLoader::loadData()
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
	catch (std::exception& e){
		std::shared_ptr<spdlog::logger> load_logger = spdlog::get("load");
		if (load_logger) {
			load_logger->error("ClientDataLoader::loadData exception: {}", e.what());
		}
		else {
			std::cerr << "load_logger is not valid. ClientDataLoader::loadData exception:" << e.what() << "\n";
		}
	}
}

std::optional<json> ClientDataLoader::getEntityData(sl::entityType type) {
	if (filePaths.empty()) { return {}; }

	auto enData = jsonFileData.find(filePaths[0]); //temp
	if (enData == jsonFileData.end()) { return {}; }

	auto jsonData = enData->second.find(type);
	if (jsonData == enData->second.end()) { return {}; }

	return jsonData->second;
}

std::optional<json> ClientDataLoader::getEntityData(std::string typeName)
{
	if (filePaths.empty()) { return {}; }

	auto enData = jsonFileData.find(filePaths[0]); //temp
	if (enData == jsonFileData.end()) { return {}; }

	auto jsonData = enData->second.find(sl::hashEntityType(typeName));
	if (jsonData == enData->second.end()) { return {}; }

	return jsonData->second;
	return std::optional<json>();
}
