#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <unordered_map>
#include <functional>
#include "PacketDataTypes.hpp"

class World;
namespace sl {
	class Entity;
}

class PlayerManager {
public:
	PlayerManager(World& world);
	void updatePlayerInputState(uint32_t playerToken, uint8_t inputState, bool pressingFlag);
private:
	World& world;
	std::unordered_map<sl::net::InputState, std::function<void(sl::Entity& entity, bool pressingFlag)>> PlayerActions;
	void initPlayerActions();
};