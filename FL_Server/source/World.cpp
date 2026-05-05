#include "pch.hpp"
#include "World.hpp"
#include "Entity.hpp"

World::World() : WorldBase(){}

World::~World() = default;

void World::onUpdate(float updateTime)
{
}

void World::addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken)
{
	if (!entity) { return; }

	playersEntity.emplace_back(std::move(entity));
	tokenToIndex.try_emplace(sessionToken, playersEntity.size()-1);
	indexToToken.try_emplace(playersEntity.size() - 1, sessionToken);
}

bool World::removePlayerEntityUsingToken(const uint32_t& sessionToken)
{
	auto it = tokenToIndex.find(sessionToken);
	if (it == tokenToIndex.end()) { return false; }

	size_t removedIdx = it->second;
	size_t lastIdx = playersEntity.size() - 1;

	if (removedIdx != lastIdx) {
		playersEntity[removedIdx] = std::move(playersEntity[lastIdx]);

		uint32_t movedToken = indexToToken[lastIdx];
		tokenToIndex[movedToken] = removedIdx;
		indexToToken[removedIdx] = movedToken;
	}

	tokenToIndex.erase(sessionToken);
	indexToToken.erase(lastIdx);
	playersEntity.pop_back();

	return true;
}

bool World::removePlayerEntityUsingIndex(const size_t& index)
{
	auto it = indexToToken.find(index);
	if (it == indexToToken.end()) { return false; }

	return removePlayerEntityUsingToken(it->second);
}
