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
		PT_InputState = 0,
		PT_Input = 1,
		PT_PlayerPosition = 2,
		PT_GameStateUpdate = 3,
	};
}