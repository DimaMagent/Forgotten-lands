#include "pch.hpp"
#include "AnimationsStorage.hpp"

AnimationsStorage::AnimationsStorage() : animationsStorage(std::make_shared<TexturesStorage>())
{
}

void AnimationsStorage::addAnimations(AnimationType type, const std::string& direction, const AnimationFrames& frames)
{
	AnimationMap map;
	map.try_emplace(direction, frames);
	animationsStorage->try_emplace(type, std::move(map));
}

bool AnimationsStorage::getAnimationFrame(AnimationType type, const std::string& direction, size_t frameIndex, sf::Texture& outTexture) const
{
	auto it1 = animationsStorage->find(type);
	if (it1 == animationsStorage->end()) {
		spdlog::get("game")->error("Animation type {} not found", type);
		return false;
	}

	auto it2 = it1->second.find(direction);
	if (it2 == it1->second.end()) {
		spdlog::get("game")->error("Animation direction {} not found", direction);
		return false;
	}

	if (frameIndex >= it2->second.size()) {
		spdlog::get("game")->error("Animation frame {} not found", frameIndex);
		return false;
	}

	outTexture = *it2->second[frameIndex];

	return true;

}

AnimationType AnimationsStorage::animationTypeFromString(const std::string& typeStr)
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
