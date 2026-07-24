#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "AnimationType.hpp"

using AnimationFrames = std::vector<std::shared_ptr<sf::Texture>>;
using AnimationMap = std::unordered_map<std::string, AnimationFrames>;
using TexturesStorage = std::unordered_map<AnimationType, AnimationMap>;


class AnimationsStorage {
public:
	AnimationsStorage();
	void addAnimations(AnimationType type, const std::string& direction, const AnimationFrames& frames);
	const std::shared_ptr<sf::Texture> getAnimationFrame(AnimationType type, const std::string& direction, size_t& frameIndex) const;
	const std::shared_ptr<sf::Texture> getAnimationFrame(AnimationType type, const sf::Vector2i& direction, size_t& frameIndex) const;
private:
	//stores textures-animations by type and direction
	std::shared_ptr<TexturesStorage> animationsStorage;

	std::string directionDetermining(const sf::Vector2i& direction) const;

};