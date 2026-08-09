#include "pch.h"
#include "CollisionSystem.hpp"
#include "SFML/System/Vector2.hpp"
#include "Entity.hpp"
#include "CollisionComponent.hpp"
#include "TransformComponent.hpp"
#include "CollisionCellMap.hpp"
#include "Aabb.hpp"
#include "WorldBase.hpp"
#include <optional>

sl::CollisionSystem::CollisionSystem(const sl::CollisionCellMap& collisionCellMap, const WorldBase& world)
	: collisionCellMap(collisionCellMap), world(world)
{
}

void sl::CollisionSystem::onUpdate(sl::Entity& entity, float updateTime) {
	currentTime += updateTime;
	if (currentTime >= updateRate) {
		currentTime -= updateRate;
		
		sl::CollisionComponent* colisComp = entity.getComponent<sl::CollisionComponent>();
		if (!colisComp) { return; }

		if (colisComp->isStaticCollisioner()) { return; }

		sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
		if (!transComp) { return; }

		sf::Vector2f position = transComp->getPosition();

		AABB aabb = colisComp->getAABB();

		std::vector<uint32_t> entityIds = collisionCellMap.getNearestEntityIdsToEntity(aabb, position, 1);

		for (uint32_t id : entityIds) {

			auto entityOpt = world.getEntityById(id);
			if (!entityOpt.has_value()) { continue; }
			if (id == entity.getGlobalId()) { continue; }

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

			//std::cout << "Collision detected between Entity " << entity.getGlobalId() << " and Entity " << id << " with CollisionType: " << static_cast<int>(type) << std::endl;
		}

	}
}