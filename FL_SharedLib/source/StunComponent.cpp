#include "pch.h"
#include "StunComponent.hpp"

sl::StunComponent::StunComponent()
{
	currentStans = StunState::None;
	flagCounterContainer = FlagCounterContainer<std::underlying_type_t<StunState>, 2>();
}

void sl::StunComponent::addWithinStun(StunState state)
{
	flagCounterContainer.AddFlag(static_cast<std::underlying_type_t<StunState>>(state));
	calculateCurrentStuns();
}

void sl::StunComponent::addExternalStun(StunState state, uint32_t stunningEnityId)
{
	activeEffects[stunningEnityId] |= state;
	calculateCurrentStuns();
}

void sl::StunComponent::removeWithinStun(StunState state)
{
	flagCounterContainer.RemoveFlag(static_cast<std::underlying_type_t<StunState>>(state));

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
	sl::net::write_uint32_t(out, TypeId);
	sl::net::write_uint32_t(out, getDeserializeDataSize());
	sl::net::write_uint8_t(out, static_cast<uint8_t>(currentStuns));
}

bool sl::StunComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }
	currentStuns = static_cast<StunState>(sl::net::read_uint8_t(out, offset));

	return true;
}

uint32_t sl::StunComponent::getSerializeDataSize() const
{
	return sizeof(uint8_t) + sizeof(TypeId) + sizeof(uint32_t);
}

uint32_t sl::StunComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);
}


