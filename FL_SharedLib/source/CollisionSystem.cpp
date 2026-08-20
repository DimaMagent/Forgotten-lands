#include "pch.h"
#include "CollisionSystem.hpp"
#include "SFML/System/Vector2.hpp"
#include "Entity.hpp"
#include "CollisionComponent.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "CollisionCellMap.hpp"
#include "EntityStorage.hpp"
#include "Aabb.hpp"
#include "WorldBase.hpp"
#include <optional>

sl::CollisionSystem::CollisionSystem(const sl::CollisionCellMap& collisionCellMap, const WorldBase& world)
	: collisionCellMap(collisionCellMap), world(world)
{
	reusableEntityIdsBuffer = std::vector<uint32_t>();
}

void sl::CollisionSystem::onUpdate(float updateTime) {
	currentTime += updateTime;
	if (currentTime >= updateRate) {
		currentTime -= updateRate;

		for (auto& entity : world.getEntities()) {
			
			if (!entity) { continue; }

			sl::CollisionComponent* colisComp = entity->getComponent<sl::CollisionComponent>();
			if (!colisComp) { continue; }

			if (colisComp->isStaticCollisioner()) { continue; }

			sl::MovementComponent* moveComp = entity->getComponent<sl::MovementComponent>();
			if (moveComp) {
				if (!moveComp->isMoving()) {
					continue;
				}
			}

			sl::TransformComponent* transComp = entity->getComponent<sl::TransformComponent>();
			if (!transComp) { continue; }

			sf::Vector2f position = transComp->getPosition();

			AABB aabb = colisComp->getAABB();

			reusableEntityIdsBuffer.clear();

			bool isSuccess = collisionCellMap.getNearestEntityIdsToEntity(aabb, position, reusableEntityIdsBuffer, SEARCH_DEPTH);

			if (!isSuccess) { continue; }

			for (uint32_t id : reusableEntityIdsBuffer) {

				auto entityOpt = world.getEntityById(id);
				if (!entityOpt.has_value()) { continue; }

				if (id == entity->getGlobalId()) { continue; }

				sl::CollisionComponent* anotherColisComp = entityOpt.value().get().getComponent<sl::CollisionComponent>();
				if (!anotherColisComp) { continue; }

				sl::TransformComponent* anotherTransComp = entityOpt.value().get().getComponent<sl::TransformComponent>();
				if (!anotherTransComp) { continue; }

				AABB otherAABB = anotherColisComp->getAABB();

				sf::Vector2f otherPosition = anotherTransComp->getPosition();

				sl::CollisionType type = colisComp->isRelativeCollisionWith(position.x, position.y, otherAABB,
					otherPosition.x, otherPosition.y);

				if (type == sl::CollisionType::None) { continue; }

				onCollisionDetected.broadcast(*entity, entityOpt.value(), type);

				//std::cout << "Collision detected between Entity " << entity->getGlobalId() << " and Entity " << id << " with CollisionType: " << static_cast<int>(type) << std::endl;
			}
		}

	}
}
