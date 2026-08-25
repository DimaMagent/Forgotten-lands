#include "pch.h"
#include "WeaponComponent.hpp"
#include "NetUtils.hpp"

sl::WeaponComponent::WeaponComponent()
{
	this->attackDistance = 0.0f;
	this->attackDegrees = 0.0f;
	this->attackDamage = 0.0f;
}

sl::WeaponComponent::WeaponComponent(float attackDistance, float attackDegrees, float attackDamage)
{
	this->attackDistance = attackDistance;
	this->attackDegrees = attackDegrees;
	this->attackDamage = attackDamage;
}

void sl::WeaponComponent::serialize(std::vector<uint8_t>& out) const
{
	sl::net::write<uint32_t>(out, TypeId);
	sl::net::write<uint32_t>(out, getDeserializeDataSize());
	sl::net::write<float>(out, attackDistance);
	sl::net::write<float>(out, attackDegrees);
	sl::net::write<float>(out, attackDamage);
}

bool sl::WeaponComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }
	attackDistance = sl::net::read<float>(out, offset);
	attackDegrees = sl::net::read<float>(out, offset);
	attackDamage = sl::net::read<float>(out, offset);

	return true;
}

uint32_t sl::WeaponComponent::getSerializeDataSize() const
{
	return sizeof(attackDistance) + sizeof(attackDegrees) + sizeof(attackDamage) + sizeof(TypeId) + sizeof(uint32_t);
}

uint32_t sl::WeaponComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);;
}
