#pragma once
#include "DataLoader.hpp"
#include <optional>

class ServerDataLoader : public sl::DataLoader {
public:
	ServerDataLoader();

	std::optional<json> getEntityData(sl::EntityType id) override;
private:
	void loadData() override;
};