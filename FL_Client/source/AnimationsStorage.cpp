#include "pch.hpp"
#include "AnimationsStorage.hpp"

void AnimationsStorage::addAnimations(AnimationType type, const sf::Vector2i& direction, const AnimationFrames& frames)
{
	AnimationMap map;
	map.try_emplace(direction, frames);
	animationsStorage->try_emplace(type, std::move(map));
}

bool AnimationsStorage::getAnimationFrame(AnimationType type, const sf::Vector2i& direction, size_t frameIndex, sf::Texture& outTexture) const
{
	auto it1 = animationsStorage->find(type);
	if (it1 == animationsStorage->end()) {
		spdlog::get("game")->error("Animation type {} not found", type);
		return false;
	}

	auto it2 = it1->second.find(direction);
	if (it2 == it1->second.end()) {
		spdlog::get("game")->error("Animation direction ({};{}) not found", direction.x, direction.y);
		return false;
	}

	if (frameIndex >= it2->second.size()) {
		spdlog::get("game")->error("Animation frame {} not found", frameIndex);
		return false;
	}

	outTexture = it2->second[frameIndex];

	return true;

}
