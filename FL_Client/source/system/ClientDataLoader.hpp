#pragma once
#include "DataLoader.hpp"

using entityData = std::unordered_map<uint32_t, json>;

class ClientDataLoader: public sl::DataLoader {
public:
	ClientDataLoader();

	std::optional<json> getEntityData(sl::EntityType id) override;
private:

	void loadData() override;
};