#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <nlohmann\json.hpp>
#include <optional>
#include <unordered_map>
#include "EntityType.hpp"


using json = nlohmann::json;

using entityData = std::unordered_map<sl::entityType, json>;

namespace sl {

	class DataLoader {
	public:
		DataLoader();
		DataLoader(const DataLoader&) = delete;
		virtual ~DataLoader();
		virtual std::optional<json> getEntityData(sl::entityType type) = 0;
		virtual std::optional<json> getEntityData(std::string typeName) = 0;

	protected:
		static std::vector<std::string> filePaths;

		//stores paths to json file and entityData
		std::unordered_map<std::string, entityData> jsonFileData;

		virtual void loadData() = 0;
	};

}