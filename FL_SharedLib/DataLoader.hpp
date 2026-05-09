#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <nlohmann\json.hpp>

using json = nlohmann::json;


class DataLoader {
public:
	DataLoader();
	DataLoader(const DataLoader&) = delete;
	~DataLoader();
	json getData(const std::string& id);
};