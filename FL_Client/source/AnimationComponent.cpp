#include "pch.hpp"
#include "AnimationComponent.hpp"
#include "AnimationsStorage.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Entity.hpp"
#include "TextureManager.hpp"

AnimationComponent::AnimationComponent(std::shared_ptr<const AnimationsStorage> animStorage)
{
	currentIndex = 0;
	currentAnimationType = AnimationType::Idle;
	currentDirection = sf::Vector2i(0, -1);
	animationsStorage = animStorage;
}

void AnimationComponent::addAllowedAnimationFrequency(AnimationType animationType, float frequency)
{
	AnimationFrequency aniumationFrequency(frequency, frequency, 0);

	frequencyOfAllowedAnimations.try_emplace(animationType, aniumationFrequency);
}

bool AnimationComponent::getCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float& frequencyOut) const
{
	auto it = frequencyOfAllowedAnimations.find(animationType);
	if (it == frequencyOfAllowedAnimations.end()) {
		return false;
	}
	frequencyOut = it->second.currentFrequencyOfAllowedAnimations;
	return true;
}

bool AnimationComponent::setCurrentFrequencyOfAnimationIfExist(AnimationType animationType, float newFrequency)
{
	auto it = frequencyOfAllowedAnimations.find(animationType);
	if (it == frequencyOfAllowedAnimations.end()) {
		return false;
	}
	it->second.currentFrequencyOfAllowedAnimations = newFrequency;
	return true;
}

bool AnimationComponent::addTimeSinceLastUpdate(AnimationType animationType, float dt)
{
	auto it = frequencyOfAllowedAnimations.find(animationType);
	if (it == frequencyOfAllowedAnimations.end()) {
		return false;
	}
	it->second.timeSinceLastUpdateOfAllowedAnimations += dt;
	return true;
}

bool AnimationComponent::getTimeSinceLastUpdate(AnimationType animationType, float& outTime)
{
	auto it = frequencyOfAllowedAnimations.find(animationType);
	if (it == frequencyOfAllowedAnimations.end()) {
		return false;
	}
	outTime = it->second.timeSinceLastUpdateOfAllowedAnimations;
	return true;
}

bool AnimationComponent::resetCurrentFrequencyOfAnimationIfExist(AnimationType resetAnimationType)
{
	auto it1 = frequencyOfAllowedAnimations.find(resetAnimationType);
	if (it1 == frequencyOfAllowedAnimations.end()) {
		return false;
	}
	it1->second.currentFrequencyOfAllowedAnimations = it1->second.baseFrequencyOfAllowedAnimations;
	return true;
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

const std::shared_ptr<sf::Texture> AnimationComponent::getCurrentAnimationFramePlayingAnimation(AnimationType type, const sf::Vector2i& direction, bool isReset)
{
	if (currentAnimationType != type || isReset) {
		currentAnimationType = type;
		currentDirection = direction;
		currentIndex = 0;
		animationPlaying = true;
	}
	else {
		if (currentDirection != direction) {
			currentDirection = direction;
		}

		size_t frameCount = animationsStorage->getFramesCount(type, direction);
		++currentIndex;
		if (currentIndex >= frameCount) {
			animationPlaying = false;
			return nullptr;
		}
	}
	return animationsStorage->getAnimationFrame(currentAnimationType, currentDirection, currentIndex);
}

size_t AnimationComponent::getFramesCountForCurrentAnimation(AnimationType type, const sf::Vector2i& direction) const
{
	return animationsStorage->getFramesCount(type, direction);
}

AnimationComponent::AnimationFrequency::AnimationFrequency(float baseFrequencyOfAllowedAnimations, float currentFrequencyOfAllowedAnimations, float timeSinceLastUpdateOfAllowedAnimations)
{
	this->baseFrequencyOfAllowedAnimations = baseFrequencyOfAllowedAnimations;
	this->currentFrequencyOfAllowedAnimations = currentFrequencyOfAllowedAnimations;
	this->timeSinceLastUpdateOfAllowedAnimations = timeSinceLastUpdateOfAllowedAnimations;
}

void AnimationComponent::initialize(sl::ComponentInitContext context) {
	auto* textureManager = context.getService<TextureManager>();
	if (!textureManager) {
		throw("ComponentInitContext does not store TextureManager");
		return;
	}
	std::shared_ptr<AnimationsStorage> animationStorage = std::make_shared<AnimationsStorage>();

	if (context.js.contains("Animations") && context.js["Animations"].is_object()) {
		for (auto& [animationName, animation] : context.js["Animations"].items()) {
			std::vector<std::shared_ptr<sf::Texture>> directionFrames;
			for (auto& [directionName, framePaths] : animation.items()) {
				for (auto& framePath : framePaths) {
					directionFrames.push_back(textureManager->getTexture(framePath));
				}
				animationStorage->addAnimations(animationTypeFromString(animationName), directionName, directionFrames);
				directionFrames.clear();
			}
		}
	}

	AnimationComponent& animComp = context.entity.addComponent<AnimationComponent>(animationStorage);

	if (context.js.contains("AllowedAnimation") && context.js["AllowedAnimation"].is_object()) {
		for (auto& [animationName, frequency] : context.js["AllowedAnimation"].items()) {
			animComp.addAllowedAnimationFrequency(animationTypeFromString(animationName), frequency);
		}
	}
}
