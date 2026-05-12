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
};