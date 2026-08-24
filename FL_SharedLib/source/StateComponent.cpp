#include "pch.h"
#include "StateComponent.hpp"
#include "NetUtils.hpp"

sl::StateComponent::StateComponent()
{
	actionState = ActionState::None;
	movementState = MovementState::None;
	lifeState = LifeState::Alive;
	stunState = StunState::None;
}

void sl::StateComponent::setCurrentActionState(ActionState newState)
{
	actionState = newState;
}

void sl::StateComponent::setCurrentMovementState(MovementState newState)
{
	movementState = newState;
}

void sl::StateComponent::setCurrentLifeState(LifeState newState)
{
	lifeState = newState;
}

void sl::StateComponent::serialize(std::vector<uint8_t>& out) const
{
	sl::net::write_uint32_t(out, TypeId);
	sl::net::write_uint32_t(out, getDeserializeDataSize());
	sl::net::write_uint8_t(out, static_cast<uint8_t>(actionState));
	sl::net::write_uint8_t(out, static_cast<uint8_t>(movementState));
	sl::net::write_uint8_t(out, static_cast<uint8_t>(lifeState));

}

bool sl::StateComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }

	actionState = static_cast<ActionState>(sl::net::read_uint8_t(out, offset));
	movementState = static_cast<MovementState>(sl::net::read_uint8_t(out, offset));
	lifeState = static_cast<LifeState>(sl::net::read_uint8_t(out, offset));

	return true;
}

uint32_t sl::StateComponent::getSerializeDataSize() const
{
	return sizeof(actionState) + sizeof(movementState) + sizeof(lifeState) + sizeof(TypeId) + sizeof(uint32_t);
}

uint32_t sl::StateComponent::getDeserializeDataSize() const
{
	return getSerializeDataSize() - sizeof(TypeId) - sizeof(uint32_t);
}