#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <nlohmann\json.hpp>
#include <optional>
#include <unordered_map>
#include "EntityType.hpp"


using json = nlohmann::json;


namespace sl {

	using entityData = std::unordered_map<uint32_t, json>;

	class DataLoader {
	public:
		DataLoader();
		DataLoader(const DataLoader&) = delete;
		virtual ~DataLoader();
		virtual std::optional<json> getEntityData(sl::EntityType id) = 0;

	protected:
		static std::vector<std::string> filePaths;
		std::unordered_map<std::string, entityData> jsonFileData;

		virtual void loadData() = 0;
	};

}