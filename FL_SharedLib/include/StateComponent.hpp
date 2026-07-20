#pragma once
#include "Component.hpp"

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


	class StateComponent : public Component {
	public:
		StateComponent();

		ActionState actionState;
		MovementState movementState;
		LifeState lifeState;


		COMPONENT_TYPE(StateComponent);
	};
}