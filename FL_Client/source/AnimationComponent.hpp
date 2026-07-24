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

	bool getCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float& outFrequency) const;

	// frequency should be as x/60
	bool setCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float newFrequency);

	bool addTimeSinceLastUpdate(AnimationType animationType, float dt);

	bool getTimeSinceLastUpdate(AnimationType animationType, float& outTime);

	bool resetCurrentFrequencyOfAnimationIfExist(AnimationType resetAnimationType);

	void resetAllAnimationFrequency();

	const std::shared_ptr<sf::Texture> getCurrentAnimationFrame(AnimationType type, const sf::Vector2i& direction);


	COMPONENT_TYPE(AnimationComponent);
private:
	std::shared_ptr<const AnimationsStorage> animationsStorage;

	AnimationType currentAnimationType;

	sf::Vector2i currentDirection;

	size_t currentIndex = 0;

	// frequency stored as x/60
	std::unordered_map<AnimationType, float> baseFrequencyOfAllowedAnimations;
	// frequency stored as x/60
	std::unordered_map<AnimationType, float> currentFrequencyOfAllowedAnimations;

	std::unordered_map<AnimationType, float> timeSinceLastUpdateOfAllowedAnimations;
};