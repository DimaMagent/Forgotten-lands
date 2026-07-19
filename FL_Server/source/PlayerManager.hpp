#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <unordered_map>
#include <functional>
#include "PacketDataTypes.hpp"
#include "SFML/System/Vector2.hpp"

class World;
namespace sl {
	class Entity;
}


class PlayerManager {
public:
	PlayerManager(World& world);
	void updatePlayerInputState(uint32_t playerToken, sf::Vector2i movementDirectionIntentions, uint8_t inputState, uint8_t inputAction);
private:
	World& world;
	std::unordered_map<sl::net::InputState, std::function<void(sl::Entity& entity)>> playerActions;
	void initPlayerActions();
	void movementUpdate(sl::Entity& entity, sf::Vector2i movementDirectionIntentions);
};