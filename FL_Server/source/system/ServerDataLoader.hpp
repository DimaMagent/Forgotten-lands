#pragma once
#include "system/DataLoader.hpp"
#include <optional>

class ServerDataLoader : public sl::DataLoader {
public:
	ServerDataLoader();

	std::optional<json> getEntityData(sl::entityType type) override;

	std::optional<json> getEntityData(std::string typeName) override;
private:
	void loadData() override;
};