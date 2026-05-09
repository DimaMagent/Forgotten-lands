#include "pch.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"

World::World() : WorldBase(){}

World::~World() = default;

void World::onUpdate(float updateTime)
{
	for (auto& entity : playerEntities) {
		if (!entity) { continue; }

		sl::MovementComponent* movComp = entity->getComponent<sl::MovementComponent>();
		if (!movComp || !movComp->isMoving()) { continue; }

		sl::TransformComponent* trComp = entity->getComponent<sl::TransformComponent>();
		if (!trComp) { continue; }

		trComp->setPosition(movComp->move(updateTime, trComp->getPosition()));
		std::cout << "entity is moving to^ " << trComp->getPosition().x << ";" << trComp->getPosition().y << "\n";
	}
}

void World::addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken)
{
	if (!entity) { return; }

	playerEntities.emplace_back(std::move(entity));
	tokenToIndex.try_emplace(sessionToken, playerEntities.size()-1);
	indexToToken.try_emplace(playerEntities.size() - 1, sessionToken);
}

bool World::removePlayerEntityUsingToken(const uint32_t& sessionToken)
{
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

bool World::removePlayerEntityUsingIndex(const size_t& index)
{
	auto it = indexToToken.find(index);
	if (it == indexToToken.end()) { return false; }

	return removePlayerEntityUsingToken(it->second);
}

std::weak_ptr<sl::Entity> World::getPlayerEntityToToken(uint32_t token) const
{
	if (auto it = tokenToIndex.find(token); it != tokenToIndex.end()) {
		auto& entity = playerEntities[it->second];
		return entity;
	}
	return {};
}
