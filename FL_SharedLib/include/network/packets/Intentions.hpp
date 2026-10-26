#pragma once
#include <SFML/System/Vector2.hpp>
#include "system/systems/Action.hpp"
#include <vector>

namespace sl {

	struct Intentions {
		sf::Vector2i movementDirectionIntentions;
		sl::Action action;
		bool isActionActive = false;
	};

}