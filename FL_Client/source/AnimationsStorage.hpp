#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
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

using AnimationFrames = std::vector<sf::Texture>;
using AnimationMap = std::unordered_map<sf::Vector2i, AnimationFrames>;
using TexturesStorage = std::unordered_map<AnimationType, AnimationMap>;


class AnimationsStorage {
public:
	void addAnimations(AnimationType type, const sf::Vector2i& direction, const AnimationFrames& frames);
	bool getAnimationFrame(AnimationType type, const sf::Vector2i& direction, size_t frameIndex, sf::Texture& outTexture) const;
private:
	//stores textures-animations by type and direction
	std::shared_ptr<TexturesStorage> animationsStorage;

};