#include "pch.h"
#include "WeaponComponent.hpp"
#include "NetUtils.hpp"

sl::WeaponComponent::WeaponComponent()
{
	this->attackDistance = 0.0f;
	this->attackDegrees = 0.0f;
}

sl::WeaponComponent::WeaponComponent(float attackDistance, float attackDegrees)
{
	this->attackDistance = attackDistance;
	this->attackDegrees = attackDegrees;
}

void sl::WeaponComponent::serialize(std::vector<uint8_t>& out) const
{
	sl::net::write_uint32_t(out, TypeId);
	sl::net::write_uint32_t(out, getDeserializeDataSize());
	sl::net::write_float(out, attackDistance);
	sl::net::write_float(out, attackDegrees);
}

bool sl::WeaponComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }
	attackDistance = sl::net::read_float(out, offset);
	attackDegrees = sl::net::read_float(out, offset);

	return true;
}

uint32_t sl::WeaponComponent::getSerializeDataSize() const
{
	return sizeof(attackDistance) + sizeof(attackDegrees) + sizeof(TypeId) + sizeof(uint32_t);
}

uint32_t sl::WeaponComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);;
}
