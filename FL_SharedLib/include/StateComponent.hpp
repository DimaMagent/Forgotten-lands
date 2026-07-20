#pragma once
#include "Component.hpp"
#include "Serializable.hpp"

namespace sl {
	enum class ActionState: uint8_t {
		None,
		MeleeAttack,
		Shoot,
		Rest,
		Talk
	};

	enum class MovementState : uint8_t {
		None,
		Walk,
		Run
	};

	enum class LifeState : uint8_t {
		Alive,
		TakingDamage,
		Death
	};


	class StateComponent : public Component, public sl::Serializable {
	public:
		StateComponent();

		ActionState actionState;
		MovementState movementState;
		LifeState lifeState;

		virtual void serialize(std::vector<uint8_t>& out) const override;
		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;
		virtual uint32_t getSerializeDataSize() const override;
		virtual uint32_t getDeserializeDataSize() const override;

		COMPONENT_TYPE(StateComponent);
	};
}