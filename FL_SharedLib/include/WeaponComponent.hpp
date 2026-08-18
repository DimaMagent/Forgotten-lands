#pragma once
#include "Component.hpp"
#include <SFML/System/Vector2.hpp>

namespace sl {

	class WeaponComponent :public Component {
	public:
		WeaponComponent(float attackDistance, float attackDegrees);

		float getAttackDistance() const { return attackDistance; }
		float getAttackDegrees() const { return attackDegrees; }


		COMPONENT_TYPE(WeaponComponent);
	private:
		float attackDistance;
		float attackDegrees;
	};

}