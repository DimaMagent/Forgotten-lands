#pragma once
#include <SFML/System/Vector2.hpp>
#include "PacketDataTypes.hpp"
#include <vector>

namespace sl {

	struct Intentions {
		sf::Vector2i movementDirectionIntentions;
		std::vector<sl::net::Action> actions;
	};

}