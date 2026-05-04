#pragma once
#include <cstdint>
namespace sl {
	enum InputState : uint8_t {
		IS_None = 0,
		IS_MoveUp = 1 << 0,
		IS_MoveDown = 1 << 1,
		IS_MoveLeft = 1 << 2,
		IS_MoveRight = 1 << 3,
	};

	enum PacketType : uint8_t {
		PT_Auth = 0,
		PT_InputState = 1,
		PT_Input = 2,
		PT_PlayerPosition = 3,
		PT_GameStateUpdate = 4,
	};
}