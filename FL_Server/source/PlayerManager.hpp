#pragma once
#include <cstdint>
#include <map>

class PlayerManager {
public:
	PlayerManager();
	void updatePlayerInputState(uint32_t playerId, uint8_t inputState, bool pressingFlag);
private:
	// Здесь можно хранить состояние игроков, например, в виде словаря playerId -> PlayerState
	// PlayerState может включать позицию, направление движения и другие параметры
};