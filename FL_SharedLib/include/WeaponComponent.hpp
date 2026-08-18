#pragma once
#include "Component.hpp"
#include <SFML/System/Vector2.hpp>

namespace sl {

	class WeaponComponent :public Component {
	public:
		WeaponComponent(float attackWidth, float attackHeight);

	private:
		sf::Vector2f attackDistance;
	};

}