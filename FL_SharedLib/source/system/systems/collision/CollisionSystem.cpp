#include "pch.h"
#include "system/systems/collision/CollisionSystem.hpp"
#include "SFML/System/Vector2.hpp"
#include "system/Entity.hpp"
#include "system/systems/collision/CollisionComponent.hpp"
#include "system/systems/TransformComponent.hpp"
#include "system/systems/movement/MovementComponent.hpp"
#include "system/systems/collision/CollisionCellMap.hpp"
#include "system/storages/EntityStorage.hpp"
#include "system/systems/collision/Aabb.hpp"
#include "system/IEntityRegistry.hpp"
#include <optional>

sl::CollisionSystem::CollisionSystem(const sl::CollisionCellMap& collisionCellMap, const sl::IEntityRegistry& entityRegistry):
	collisionCellMap(collisionCellMap), entityRegistry(entityRegistry)
{
}

void sl::CollisionSystem::update(float updateTime, const sl::Entity& entity) {
	currentTime += updateTime;
	if (currentTime >= updateRate) {
		currentTime -= updateRate;

		sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
		if (!colisComp) { return; }

		if (colisComp->isStaticCollisioner()) { return; }

		sl::MovementComponent* moveComp = entity.getComponent<sl::MovementComponent>();
		if (moveComp) {
			if (!moveComp->isMoving()) {
				return;
			}
		}

		sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
		if (!transComp) { return; }

		sf::Vector2f position = transComp->getPosition();

		AABB aabb = colisComp->getAABB();

		reusableEntityIdsBuffer.clear();

		bool isSuccess = collisionCellMap.getNearestEntityIdsToEntity(aabb, position, reusableEntityIdsBuffer, SEARCH_DEPTH);

		if (!isSuccess) { return; }

		for (sl::EntityId id : reusableEntityIdsBuffer) {

			auto entityOpt = entityRegistry.getEntityById(id);
			if (!entityOpt.has_value()) { continue; }

			if (id == entity.getId()) { continue; }

			sl::CollisionComponent* anotherColisComp = entityOpt.value().get().getComponent<sl::CollisionComponent>();
			if (!anotherColisComp) { continue; }

			sl::TransformComponent* anotherTransComp = entityOpt.value().get().getComponent<sl::TransformComponent>();
			if (!anotherTransComp) { continue; }

			AABB otherAABB = anotherColisComp->getAABB();

			sf::Vector2f otherPosition = anotherTransComp->getPosition();

			sl::CollisionType type = colisComp->isRelativeCollisionWith(position.x, position.y, otherAABB,
				otherPosition.x, otherPosition.y);

			if (type == sl::CollisionType::None) { continue; }

			onCollisionDetected.broadcast(entity, entityOpt.value(), type);

			//std::cout << "Collision detected between Entity " << entity->getGlobalId().ID << " and Entity " << id << " with CollisionType: " << static_cast<int>(type) << std::endl;
		}


	}
}

sl::systemPtr sl::CollisionSystem::initialize(sl::SystemCreatorContext context) {
	return std::make_unique<sl::CollisionSystem>(context.collisionCellMap, context.entityRegistry);
}
