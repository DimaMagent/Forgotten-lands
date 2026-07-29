#include "pch.hpp"
#include "ClientDataLoader.hpp"
#include <fstream>
#include "EntityType.hpp"

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

				for (uint32_t entityType = 0; sl::EntityTypeToString(static_cast<sl::EntityType>(entityType)) != "undefined"; ++entityType) {
					auto& EntityData = data.at(sl::EntityTypeToString(static_cast<sl::EntityType>(entityType)));
					jsonFileData[path].try_emplace(entityType, EntityData);
				}

				file.close();
			}
			else {
				throw std::runtime_error("Could not open file with current path: " + path);
				return;
			}
		}
	}
	catch(std::exception& e){
		std::shared_ptr<spdlog::logger> load_logger = spdlog::get("load");
		if (load_logger) {
			load_logger->error("ClientDataLoader::loadData exception: {}", e.what());
		}
		else {
			std::cerr << "load_logger is not valid.\nClientDataLoader::loadData exception:" << e.what() << "\n";
		}
	}
}

std::optional<json> ClientDataLoader::getEntityData(sl::EntityType id) {
	auto enData = jsonFileData.find("manifests/ClientCharacterComponents.json"); //temp
	if (enData == jsonFileData.end()) { return {}; }

	auto jsonData = enData->second.find(static_cast<uint32_t>(id));
	if (jsonData == enData->second.end()) { return {}; }

	return jsonData->second;
}
