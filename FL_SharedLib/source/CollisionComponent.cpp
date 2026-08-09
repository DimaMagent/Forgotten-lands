#include "pch.h"
#include "CollisionComponent.hpp"

sl::CollisionComponent::CollisionComponent(float width, float height, bool isSataticCollisioner, CollisionType collisionType)
	:isSataticCollisioner(isSataticCollisioner), collisionType(collisionType)
{
	aabb.minX = 0.f;
	aabb.minY = 0.f;
	aabb.maxX = width;
	aabb.maxY = height;
}

sl::CollisionType sl::CollisionComponent::isRelativeCollisionWith(float posX, float posY, AABB otherAABB, float otherPosX, float otherPosY) const
{
	AABB relativeAABB = getRelativeAABB(this->aabb, posX, posY);
	AABB otherRelativeAABB = getRelativeAABB(otherAABB, otherPosX, otherPosY);
	if (relativeAABB.intersects(otherRelativeAABB)) { return collisionType; }

	return CollisionType::None;
}

sl::AABB sl::CollisionComponent::getRelativeAABB(AABB aabb, float posX, float posY) const
{
	return AABB(posX + aabb.minX, posY + aabb.minY, posX + aabb.maxX, posY + aabb.maxY);
}

sl::CollisionType sl::stringToCollisionType(const std::string& str)
{
	if (str == "Block") return CollisionType::Block;
	if (str == "Overlapped") return CollisionType::Overlapped;
	if (str == "Ignore") return CollisionType::Ignore;
	return CollisionType::None;
}
