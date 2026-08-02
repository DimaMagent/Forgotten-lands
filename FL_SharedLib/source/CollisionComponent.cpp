#include "pch.h"
#include "CollisionComponent.hpp"

sl::CollisionComponent::CollisionComponent(float width, float height, bool isSataticCollisioner)
	:isSataticCollisioner(isSataticCollisioner)
{
	aabb.topX = 0.f;
	aabb.topY = 0.f;
	aabb.downX = width;
	aabb.downY = height;
}
