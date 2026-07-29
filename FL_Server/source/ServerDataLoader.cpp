#include "pch.hpp"
#include "ServerDataLoader.hpp"
#include <fstream>
#include "EntityType.hpp"

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
	catch (std::exception& e) {
		std::shared_ptr<spdlog::logger> load_logger = spdlog::get("load");
		if (load_logger) {
			load_logger->error("ServerDataLoader::loadData exception: {}", e.what());
		}
		else {
			std::cerr << "load_logger is not valid.\ServerDataLoader::loadData exception:" << e.what() << "\n";
		}
	}
}

std::optional<json> ServerDataLoader::getEntityData(sl::EntityType id) {
	auto enData = jsonFileData.find("manifests/Characters.json"); //temp
	if (enData == jsonFileData.end()) { return {}; }

	auto jsonData = enData->second.find(static_cast<uint32_t>(id));
	if (jsonData == enData->second.end()) { return {}; }

	return jsonData->second;
}
