#include "pch.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Serializer.hpp"

World::World() : WorldBase(), serializer(std::make_unique<Serializer>(OnUpdate, playerEntityStorage)){}

World::~World() = default;

void World::onUpdate(float updateTime)
{
	for (auto& entity : playerEntityStorage.playerEntities) {
		if (!entity) { continue; }

		sl::MovementComponent* movComp = entity->getComponent<sl::MovementComponent>();
		if (!movComp || !movComp->isMoving()) { continue; }

		sl::TransformComponent* trComp = entity->getComponent<sl::TransformComponent>();
		if (!trComp) { continue; }

		trComp->setPosition(movComp->move(updateTime, trComp->getPosition()));
	}
	OnUpdate.broadcast(updateTime);
}

void World::addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken)
{
	if (!entity) { return; }

	playerEntityStorage.playerEntities.emplace_back(std::move(entity));
	playerEntityStorage.tokenToIndex.try_emplace(sessionToken, playerEntityStorage.playerEntities.size()-1);
	playerEntityStorage.indexToToken.try_emplace(playerEntityStorage.playerEntities.size() - 1, sessionToken);
}

bool World::removePlayerEntityUsingToken(const uint32_t& sessionToken)
{
	auto it = playerEntityStorage.tokenToIndex.find(sessionToken);
	if (it == playerEntityStorage.tokenToIndex.end()) { return false; }

	size_t removedIdx = it->second;
	size_t lastIdx = playerEntityStorage.playerEntities.size() - 1;

	if (removedIdx != lastIdx) {
		playerEntityStorage.playerEntities[removedIdx] = std::move(playerEntityStorage.playerEntities[lastIdx]);

		uint32_t movedToken = playerEntityStorage.indexToToken[lastIdx];
		playerEntityStorage.tokenToIndex[movedToken] = removedIdx;
		playerEntityStorage.indexToToken[removedIdx] = movedToken;
	}

	playerEntityStorage.tokenToIndex.erase(sessionToken);
	playerEntityStorage.indexToToken.erase(lastIdx);
	playerEntityStorage.playerEntities.pop_back();

	return true;
}

bool World::removePlayerEntityUsingIndex(const size_t& index)
{
	auto it = playerEntityStorage.indexToToken.find(index);
	if (it == playerEntityStorage.indexToToken.end()) { return false; }

	return removePlayerEntityUsingToken(it->second);
}

std::weak_ptr<sl::Entity> World::getPlayerEntityToToken(uint32_t token) const
{
	if (auto it = playerEntityStorage.tokenToIndex.find(token); it != playerEntityStorage.tokenToIndex.end()) {
		auto& entity = playerEntityStorage.playerEntities[it->second];
		return entity;
	}
	return {};
}
