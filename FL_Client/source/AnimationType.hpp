#pragma once
#include <cstdint>

enum class AnimationType : uint8_t {
	None,
	Walk,
	Run,
	Attack,
	Rest,
	Death,
	TakingDamage,
	Talk
};