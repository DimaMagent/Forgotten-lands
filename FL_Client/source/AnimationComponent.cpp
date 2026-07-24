#include "pch.hpp"
#include "AnimationComponent.hpp"

void AnimationComponent::addAllowedAnimationSpeed(AnimationType animationType, float frequency)
{
	baseFrequencyOfAllowedAnimations.try_emplace(animationType, frequency);
	currentFrequencyOfAllowedAnimations.try_emplace(animationType, frequency);
}

bool AnimationComponent::getCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float& frequencyOut) const
{
	auto& it = currentFrequencyOfAllowedAnimations.find(animationType);
	if (it == currentFrequencyOfAllowedAnimations.end()) {
		return false;
	}
	frequencyOut = it->second;
	return true;
}

bool AnimationComponent::setCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float newFrequency)
{
	auto& it = currentFrequencyOfAllowedAnimations.find(animationType);
	if (it == currentFrequencyOfAllowedAnimations.end()) {
		return false;
	}
	it->second = newFrequency;
	return true;
}

bool AnimationComponent::resetCurrentFrequencyOfAnimationIfExist(AnimationType resetAnimationType)
{
	auto& it1 = currentFrequencyOfAllowedAnimations.find(resetAnimationType);
	auto& it2 = baseFrequencyOfAllowedAnimations.find(resetAnimationType);
	if (it1 == currentFrequencyOfAllowedAnimations.end() || it2 == baseFrequencyOfAllowedAnimations.end()) {
		return false;
	}
	it1->second = it2->second;
	return true;
}

void AnimationComponent::resetAllAnimationFrequency()
{
	currentFrequencyOfAllowedAnimations = baseFrequencyOfAllowedAnimations;
}
