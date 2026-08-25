#include "pch.h"
#include "CollisionComponent.hpp"

sl::CollisionComponent::CollisionComponent()
{
	aabb = AABB();
	staticCollisioner = true;
	collisionType = CollisionType::None;
}

sl::CollisionComponent::CollisionComponent(float width, float height, bool staticCollisioner, CollisionType collisionType)
	:staticCollisioner(staticCollisioner), collisionType(collisionType)
{
	aabb = AABB(0.f, 0.f, width, height);

}

sl::CollisionType sl::CollisionComponent::isRelativeCollisionWith(float posX, float posY, const AABB& otherAABB, float otherPosX, float otherPosY) const
{
	AABB relativeAABB = getRelativeAABB(this->aabb, posX, posY);
	AABB otherRelativeAABB = getRelativeAABB(otherAABB, otherPosX, otherPosY);
	if (relativeAABB.intersects(otherRelativeAABB)) { return collisionType; }

	return CollisionType::None;
}

void sl::CollisionComponent::serialize(std::vector<uint8_t>& out) const
{
	sl::net::write<uint32_t>(out, TypeId);
	sl::net::write<uint32_t>(out, getDeserializeDataSize());
	writeAABB(out, aabb);
	sl::net::write<std::underlying_type_t<CollisionType>>(out, static_cast<std::underlying_type_t<CollisionType>>(collisionType));
	sl::net::write<uint8_t>(out, staticCollisioner);

}

bool sl::CollisionComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }
	aabb = readAABB(out, offset);
	collisionType = static_cast<CollisionType>(sl::net::read<std::underlying_type_t<CollisionType>>(out, offset));
	staticCollisioner = sl::net::read<uint8_t>(out, offset);

	return true;
}

uint32_t sl::CollisionComponent::getSerializeDataSize() const
{
	return sizeof(aabb.minX) + sizeof(aabb.minY)
		+ sizeof(aabb.maxX) + sizeof(aabb.maxY)
		+ sizeof(collisionType) + sizeof(staticCollisioner)
		+ sizeof(TypeId) + sizeof(uint32_t);
}

uint32_t sl::CollisionComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);;
}

sl::AABB sl::CollisionComponent::getRelativeAABB(const AABB& aabb, float posX, float posY) const
{
	return AABB(posX + aabb.minX, posY + aabb.minY, posX + aabb.maxX, posY + aabb.maxY);
}

void sl::CollisionComponent::writeAABB(std::vector<uint8_t>& out, const AABB& aabb) const
{
	sl::net::write<float>(out, aabb.minX);
	sl::net::write<float>(out, aabb.minY);
	sl::net::write<float>(out, aabb.maxX);
	sl::net::write<float>(out, aabb.maxY);
}

sl::AABB sl::CollisionComponent::readAABB(const std::vector<uint8_t>& in, size_t& offset)
{
	sl::AABB aabb;

	aabb.minX = sl::net::read<float>(in, offset);
	aabb.minY = sl::net::read<float>(in, offset);
	aabb.maxX = sl::net::read<float>(in, offset);
	aabb.maxY = sl::net::read<float>(in, offset);

	return aabb;
}

sl::CollisionType sl::stringToCollisionType(const std::string& str)
{
	if (str == "Block") return CollisionType::Block;
	if (str == "Overlapped") return CollisionType::Overlapped;
	if (str == "Ignore") return CollisionType::Ignore;
	return CollisionType::None;
}
