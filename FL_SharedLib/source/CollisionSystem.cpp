#include "pch.h"
#include "CollisionSystem.hpp"
#include "Entity.hpp"
#include "CollisionComponent.hpp"
#include "TransformComponent.hpp"
#include "Aabb.hpp"
#include "CollisionCellMap.hpp"

sl::CollisionSystem::CollisionSystem(const sl::ColisionCellMap& collisionCellMap) : collisionCellMap(collisionCellMap)
{
}

void sl::CollisionSystem::onUpdate(sl::Entity& entity, float updateTime) {
	currentTime += updateTime;
	if (currentTime >= updateRate) {
		currentTime -= updateRate;
		
		sl::CollisionComponent* colisComp= entity.getComponent<sl::CollisionComponent>();
		if (!colisComp) { return; }

		if (colisComp->isStaticCollisioner()) { return; }

		sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
		if (!transComp) { return; }


	}
}