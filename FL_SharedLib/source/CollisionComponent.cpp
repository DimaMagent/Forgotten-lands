#include "pch.h"
#include "CollisionComponent.hpp"

sl::CollisionComponent::CollisionComponent(float width, float height, bool isSataticCollisioner, CollisionType collisionType)
	:isSataticCollisioner(isSataticCollisioner), collisionType(collisionType)
{
	aabb.topX = 0.f;
	aabb.topY = 0.f;
	aabb.downX = width;
	aabb.downY = height;
}

sl::CollisionType sl::CollisionComponent::isRelativeCollisionWith(float posX, float posY, AABB other) const
{
	AABB relativeAABB = getRelativeAABB(posX, posY);
	if (relativeAABB.intersects(other)) { return collisionType; }
	return CollisionType::None;
}

sl::CollisionType sl::stringToCollisionType(const std::string& str)
{
	if (str == "Block") return CollisionType::Block;
	if (str == "Overlapped") return CollisionType::Overlapped;
	if (str == "Ignore") return CollisionType::Ignore;
	return CollisionType::None;
}
