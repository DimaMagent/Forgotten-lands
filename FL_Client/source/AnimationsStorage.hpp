#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"


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

using AnimationFrames = std::vector<std::shared_ptr<sf::Texture>>;
using AnimationMap = std::unordered_map<std::string, AnimationFrames>;
using TexturesStorage = std::unordered_map<AnimationType, AnimationMap>;


class AnimationsStorage {
public:
	AnimationsStorage();
	void addAnimations(AnimationType type, const std::string& direction, const AnimationFrames& frames);
	bool getAnimationFrame(AnimationType type, const std::string& direction, size_t frameIndex, sf::Texture& outTexture) const;
private:
	//stores textures-animations by type and direction
	std::shared_ptr<TexturesStorage> animationsStorage;

};