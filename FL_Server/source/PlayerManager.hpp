#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <unordered_map>
#include <functional>
#include "PacketDataTypes.hpp"
#include "SFML/System/Vector2.hpp"
#include "Intentions.hpp"

class World;
namespace sl {
	class Entity;
}


class PlayerManager {
public:
	PlayerManager(World& world);
	void updatePlayerInputState(uint32_t playerToken, sl::Intentions intentions);
private:
	World& world;
	std::unordered_map<sl::net::Action, std::function<void(sl::Entity& entity)>> playerActions;
	void initPlayerActions();
	void movementUpdate(sl::Entity& entity, sf::Vector2i movementDirectionIntentions);
};