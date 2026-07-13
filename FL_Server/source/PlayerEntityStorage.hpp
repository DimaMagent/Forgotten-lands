#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
namespace sl {
	class Entity;
}
struct PlayerEntityStorage {
	std::vector<std::shared_ptr<sl::Entity>> playerEntities;
	std::unordered_map<uint32_t, size_t> tokenToIndex;
	std::unordered_map<size_t, uint32_t> indexToToken;

	void addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken);
	bool removePlayerEntityUsingToken(const uint32_t& sessionToken);
	bool removePlayerEntityUsingIndex(const size_t& index);
	std::weak_ptr<sl::Entity> getPlayerEntityToToken(uint32_t token) const;
};