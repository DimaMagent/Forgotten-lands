#include "pch.hpp"
#include "PlayerEntityStorage.hpp"
#include "Entity.hpp"

void PlayerEntityStorage::addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken) {

	if (!entity) { return; }

	playerEntities.emplace_back(std::move(entity));
	tokenToIndex.try_emplace(sessionToken, playerEntities.size() - 1);
	indexToToken.try_emplace(playerEntities.size() - 1, sessionToken);
}

bool PlayerEntityStorage::removePlayerEntityUsingToken(const uint32_t& sessionToken) {
	auto it = tokenToIndex.find(sessionToken);
	if (it == tokenToIndex.end()) { return false; }

	size_t removedIdx = it->second;
	size_t lastIdx = playerEntities.size() - 1;

	if (removedIdx != lastIdx) {
		playerEntities[removedIdx] = std::move(playerEntities[lastIdx]);

		uint32_t movedToken = indexToToken[lastIdx];
		tokenToIndex[movedToken] = removedIdx;
		indexToToken[removedIdx] = movedToken;
	}

	tokenToIndex.erase(sessionToken);
	indexToToken.erase(lastIdx);
	playerEntities.pop_back();

	return true;
}

bool PlayerEntityStorage::removePlayerEntityUsingIndex(const size_t& index) {
	auto it = indexToToken.find(index);
	if (it == indexToToken.end()) { return false; }

	return removePlayerEntityUsingToken(it->second);
}

std::weak_ptr<sl::Entity> PlayerEntityStorage::getPlayerEntityToToken(uint32_t token) const {
	if (auto it = tokenToIndex.find(token); it != tokenToIndex.end()) {
		auto& entity = playerEntities[it->second];
		return entity;
	}
	return {};
}