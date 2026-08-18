#include "pch.h"
#include "HealthComponent.hpp"
#include "NetUtils.hpp"
#include "Utils.hpp"

sl::HealthComponent::HealthComponent()
{
	maxHealth = 0.0f;
	currentHealth = maxHealth;
}

sl::HealthComponent::HealthComponent(float maxHealth)
{
	this->maxHealth = maxHealth;
	currentHealth = maxHealth;
}

void sl::HealthComponent::takeDamage(float damage)
{
	currentHealth = sl::boundNumber<float>(currentHealth - damage, 0.f, maxHealth);
}

void sl::HealthComponent::serialize(std::vector<uint8_t>& out) const
{
	sl::net::write_uint32_t(out, TypeId);
	sl::net::write_uint32_t(out, getDeserializeDataSize());
	sl::net::write_float(out, maxHealth);
	sl::net::write_float(out, currentHealth);
}

bool sl::HealthComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }
	maxHealth = sl::net::read_float(out, offset);
	currentHealth = sl::net::read_float(out, offset);

	return true;
}

uint32_t sl::HealthComponent::getSerializeDataSize() const
{
	return sizeof(maxHealth) + sizeof(currentHealth) + sizeof(TypeId) + sizeof(uint32_t);
}

uint32_t sl::HealthComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);
}
