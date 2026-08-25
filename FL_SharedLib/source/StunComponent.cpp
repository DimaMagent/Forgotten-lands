#include "pch.h"
#include "StunComponent.hpp"
#include <type_traits>

sl::StunComponent::StunComponent()
{
	currentStuns = StunState::None;
}

void sl::StunComponent::addWithinStun(StunState state)
{
	flagCounterContainer.AddFlag(state);
	calculateCurrentStuns();
}

void sl::StunComponent::addExternalStun(StunState state, uint32_t stunningEnityId)
{
	activeEffects[stunningEnityId] |= state;
	calculateCurrentStuns();
}

void sl::StunComponent::removeWithinStun(StunState state)
{
	flagCounterContainer.RemoveFlag(state);

	calculateCurrentStuns();
}

void sl::StunComponent::removeExternalStun(StunState state, uint32_t stunningEnityId)
{
	auto it = activeEffects.find(stunningEnityId);

	if (it == activeEffects.end()) { return; }

	it->second &= ~state;

	if (it->second == StunState::None) { activeEffects.erase(it); }

	calculateCurrentStuns();
}

void sl::StunComponent::removeAllStunsAppliedByEntityId(uint32_t stunningEnityId)
{
	auto it = activeEffects.find(stunningEnityId);

	if (it == activeEffects.end()) { return; }

	activeEffects.erase(it);

	calculateCurrentStuns();
}

bool sl::StunComponent::hasStun(StunState state) const
{
	return (currentStuns & state) != StunState::None;
}

void sl::StunComponent::calculateCurrentStuns()
{
	currentStuns = StunState::None;

	for (const auto& [source, state] : activeEffects) {
		currentStuns |= state;
	}
	currentStuns |= static_cast<StunState>(flagCounterContainer.GetMask());

}

void sl::StunComponent::serialize(std::vector<uint8_t>& out) const
{
	sl::net::write<uint32_t>(out, TypeId);
	sl::net::write<uint32_t>(out, getDeserializeDataSize());
	sl::net::write<std::underlying_type_t<StunState>>(out, static_cast<std::underlying_type_t<StunState>>(currentStuns));
}

bool sl::StunComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }
	currentStuns = static_cast<StunState>(sl::net::read<std::underlying_type_t<StunState>>(out, offset));

	return true;
}

uint32_t sl::StunComponent::getSerializeDataSize() const
{
	return sizeof(StunState) + sizeof(TypeId) + sizeof(uint32_t);
}

uint32_t sl::StunComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);
}


