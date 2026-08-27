#pragma once
#include "AnimationType.hpp"
#include "Component.hpp"
#include <memory>
#include <unordered_map>

class AnimationsStorage;

namespace sf {
	class Texture;
}

class AnimationComponent : public sl::Component {
public:
	AnimationComponent(std::shared_ptr<const AnimationsStorage> animStorage);

	// frequency should be as x/60
	void addAllowedAnimationFrequency(AnimationType animationType, float frequency);

	[[nodiscard]] bool getCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float& outFrequency) const;

	// frequency should be as x/60
	[[nodiscard]] bool setCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float newFrequency);

	[[nodiscard]] bool addTimeSinceLastUpdate(AnimationType animationType, float dt);

	[[nodiscard]] bool getTimeSinceLastUpdate(AnimationType animationType, float& outTime);

	[[nodiscard]] bool resetCurrentFrequencyOfAnimationIfExist(AnimationType resetAnimationType);

	bool isAnimationPlaying() const { return animationPlaying; }

	AnimationType getCurrentAnimationType() const { return currentAnimationType; }

	const std::shared_ptr<sf::Texture> getCurrentAnimationFrame(AnimationType type, const sf::Vector2i& direction);

	const std::shared_ptr<sf::Texture> getCurrentAnimationFramePlayingAnimation(AnimationType type, const sf::Vector2i& direction, bool isReset = false);

	size_t getFramesCountForCurrentAnimation(AnimationType type, const sf::Vector2i& direction) const;

	COMPONENT_TYPE(AnimationComponent);
private:

	struct AnimationFrequency {
		AnimationFrequency(float baseFrequencyOfAllowedAnimations, float currentFrequencyOfAllowedAnimations, float timeSinceLastUpdateOfAllowedAnimations);
		float baseFrequencyOfAllowedAnimations;
		float currentFrequencyOfAllowedAnimations;
		float timeSinceLastUpdateOfAllowedAnimations;
	};

	std::shared_ptr<const AnimationsStorage> animationsStorage;

	AnimationType currentAnimationType;

	sf::Vector2i currentDirection;

	size_t currentIndex = 0;
	
	bool animationPlaying = false;

	// frequency stored as x/60
	std::unordered_map<AnimationType, AnimationFrequency> frequencyOfAllowedAnimations;

	REGISTER_COMPONENT();
};