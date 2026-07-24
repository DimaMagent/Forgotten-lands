#pragma once
#include "AnimationType.hpp"
#include "Component.hpp"
#include <unordered_map>

class AnimationComponent : public sl::Component {
public:
	AnimationComponent() = default;

	// frequency should be as x/60
	void addAllowedAnimationSpeed(AnimationType animationType, float frequency);

	bool getCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float& frequencyOut) const;

	// frequency should be as x/60
	bool setCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float newFrequency);

	bool resetCurrentFrequencyOfAnimationIfExist(AnimationType resetAnimationType);

	void resetAllAnimationFrequency();

	COMPONENT_TYPE(AnimationComponent);
private:
	// frequency stored as x/60
	std::unordered_map<AnimationType, float> baseFrequencyOfAllowedAnimations;
	// frequency stored as x/60
	std::unordered_map<AnimationType, float> currentFrequencyOfAllowedAnimations;
};