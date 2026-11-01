#pragma once
#include "system/DataLoader.hpp"

using entityData = std::unordered_map<uint32_t, json>;

class ClientDataLoader: public sl::DataLoader {
public:
	ClientDataLoader();

	std::optional<json> getEntityData(sl::entityType type) override;

	std::optional<json> getEntityData(std::string typeName) override;
private:

	void loadData() override;
};