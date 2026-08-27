#include "pch.h"
#include "StateComponent.hpp"
#include "NetUtils.hpp"
#include <type_traits>
#include "Entity.hpp"

sl::StateComponent::StateComponent()
{
	actionState = ActionState::None;
	movementState = MovementState::None;
	lifeState = LifeState::Alive;
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
	sl::net::write<uint32_t>(out, TypeId);
	sl::net::write<uint32_t>(out, getDeserializeDataSize());
	sl::net::write<std::underlying_type_t<ActionState>>(out, static_cast<std::underlying_type_t<ActionState>>(actionState));
	sl::net::write<std::underlying_type_t<MovementState>>(out, static_cast<std::underlying_type_t<MovementState>>(movementState));
	sl::net::write<std::underlying_type_t<LifeState>>(out, static_cast<std::underlying_type_t<LifeState>>(lifeState));

}

bool sl::StateComponent::deserialize(const std::vector<uint8_t>& out, size_t& offset)
{
	if (offset + getDeserializeDataSize() > out.size()) { return false; }

	actionState = static_cast<ActionState>(sl::net::read<std::underlying_type_t<ActionState>>(out, offset));
	movementState = static_cast<MovementState>(sl::net::read<std::underlying_type_t<MovementState>>(out, offset));
	lifeState = static_cast<LifeState>(sl::net::read<std::underlying_type_t<LifeState>>(out, offset));

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

void sl::StateComponent::initialize(sl::ComponentInitContext context) {
	context.entity.addComponent<sl::StateComponent>();
}