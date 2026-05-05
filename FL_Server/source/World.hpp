#pragma once
#include "WorldBase.hpp"
#include <unordered_map>
#include <cstdint>

namespace sl {
	class Entity;
}

class World : public sl::WorldBase {
public:
	World();
	~World();
protected:
	virtual void onUpdate(float updateTime) override;
	void addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken);
	bool removePlayerEntityUsingToken(const uint32_t& sessionToken);
	bool removePlayerEntityUsingIndex(const size_t& index);
private:
	std::vector<std::shared_ptr<sl::Entity>> playersEntity;
	std::unordered_map<uint32_t, size_t> tokenToIndex;
	std::unordered_map<size_t, uint32_t> indexToToken;
};
