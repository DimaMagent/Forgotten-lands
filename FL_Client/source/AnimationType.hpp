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

constexpr AnimationType animationTypeFromString(const std::string& typeStr)
{
	if (typeStr == "Walk") return AnimationType::Walk;
	if (typeStr == "Run") return AnimationType::Run;
	if (typeStr == "Attack") return AnimationType::Attack;
	if (typeStr == "Rest") return AnimationType::Rest;
	if (typeStr == "Death") return AnimationType::Death;
	if (typeStr == "TakingDamage") return AnimationType::TakingDamage;
	if (typeStr == "Talk") return AnimationType::Talk;
	return AnimationType::None;

}