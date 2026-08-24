#include "pch.hpp"
#include "AnimationComponent.hpp"
#include "AnimationsStorage.hpp"
#include "SFML/Graphics/Texture.hpp"

AnimationComponent::AnimationComponent(std::shared_ptr<const AnimationsStorage> animStorage)
{
	currentIndex = 0;
	currentAnimationType = AnimationType::Idle;
	currentDirection = sf::Vector2i(0, -1);
	animationsStorage = animStorage;
}

void AnimationComponent::addAllowedAnimationFrequency(AnimationType animationType, float frequency)
{
	baseFrequencyOfAllowedAnimations.try_emplace(animationType, frequency);
	currentFrequencyOfAllowedAnimations.try_emplace(animationType, frequency);
	timeSinceLastUpdateOfAllowedAnimations.try_emplace(animationType, 0);
}

bool AnimationComponent::getCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float& frequencyOut) const
{
	auto it = currentFrequencyOfAllowedAnimations.find(animationType);
	if (it == currentFrequencyOfAllowedAnimations.end()) {
		return false;
	}
	frequencyOut = it->second;
	return true;
}

bool AnimationComponent::setCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float newFrequency)
{
	auto it = currentFrequencyOfAllowedAnimations.find(animationType);
	if (it == currentFrequencyOfAllowedAnimations.end()) {
		return false;
	}
	it->second = newFrequency;
	return true;
}

bool AnimationComponent::addTimeSinceLastUpdate(AnimationType animationType, float dt)
{
	auto it = timeSinceLastUpdateOfAllowedAnimations.find(animationType);
	if (it == timeSinceLastUpdateOfAllowedAnimations.end()) {
		return false;
	}
	it->second += dt;
	return true;
}

bool AnimationComponent::getTimeSinceLastUpdate(AnimationType animationType, float& outTime)
{
	auto it = timeSinceLastUpdateOfAllowedAnimations.find(animationType);
	if (it == timeSinceLastUpdateOfAllowedAnimations.end()) {
		return false;
	}
	outTime = it->second;
	return true;
}

bool AnimationComponent::resetCurrentFrequencyOfAnimationIfExist(AnimationType resetAnimationType)
{
	auto it1 = currentFrequencyOfAllowedAnimations.find(resetAnimationType);
	auto it2 = baseFrequencyOfAllowedAnimations.find(resetAnimationType);
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

const std::shared_ptr<sf::Texture> AnimationComponent::getCurrentAnimationFrame(AnimationType type, const sf::Vector2i& direction){
	if (currentAnimationType == type && currentDirection == direction) {
		size_t frameCount = animationsStorage->getFramesCount(type, direction);
		currentIndex = ++currentIndex % frameCount;
	}
	else {
		currentAnimationType = type;
		currentDirection = direction;
		currentIndex = 0;
	}

	return animationsStorage->getAnimationFrame(currentAnimationType, currentDirection, currentIndex);
}

const std::shared_ptr<sf::Texture> AnimationComponent::getCurrentAnimationFramePlayingAnimation(AnimationType type, const sf::Vector2i& direction)
{
	if (currentAnimationType == type) {
		if (currentDirection != direction) {
			currentDirection = direction;
		}
		size_t frameCount = animationsStorage->getFramesCount(type, direction);
		if (++currentIndex >= frameCount) {
			return nullptr;
		}
	}
	else {
		currentAnimationType = type;
		currentDirection = direction;
		currentIndex = 0;
	}

	return animationsStorage->getAnimationFrame(currentAnimationType, currentDirection, currentIndex);
}
