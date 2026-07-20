#include "pch.hpp"
#include "AnimationsStorage.hpp"

AnimationsStorage::AnimationsStorage() : animationsStorage(std::make_shared<TexturesStorage>())
{
}

void AnimationsStorage::addAnimations(AnimationType type, const std::string& direction, const AnimationFrames& frames)
{
	(*animationsStorage)[type][direction] = frames;
}

bool AnimationsStorage::getAnimationFrame(AnimationType type, const std::string& direction, size_t& frameIndex, sf::Texture& outTexture) const
{
	auto it1 = animationsStorage->find(type);
	if (it1 == animationsStorage->end()) {
		spdlog::get("game")->error("Animation type {} not found", static_cast<int>(type));
		return false;
	}

	auto it2 = it1->second.find(direction);
	if (it2 == it1->second.end()) {
		spdlog::get("game")->error("Animation direction {} not found", direction);
		return false;
	}

	if (frameIndex >= it2->second.size()) {
		frameIndex = frameIndex % it2->second.size();
	}

	outTexture = *it2->second[frameIndex];

	return true;
}

bool AnimationsStorage::getAnimationFrame(AnimationType type, const sf::Vector2i& direction, size_t& frameIndex, sf::Texture& outTexture) const
{
	std::string dir = directionDetermining(direction);
	if (dir == "None") {
		std::shared_ptr<spdlog::logger> load_logger = spdlog::get("load");
		load_logger->warn("AnimationsStorage::addAnimations wrong animation direction: ({} ; {})", direction.x, direction.y);
		return false;
	}

	return getAnimationFrame(type, dir, frameIndex, outTexture);
}

std::string AnimationsStorage::directionDetermining(const sf::Vector2i& direction) const
{
	if (direction == sf::Vector2i(0, -1)) { return "Forward"; }
	if (direction == sf::Vector2i(-1, -1)) { return "ForwardLeft"; }
	if (direction == sf::Vector2i(1, -1)) { return "ForwardRight"; }
	if (direction == sf::Vector2i(-1, 0)) { return "Left"; }
	if (direction == sf::Vector2i(1, 0)) { return "Right"; }
	if (direction == sf::Vector2i(0, 1)) { return "Backward"; }
	if (direction == sf::Vector2i(-1, 1)) { return "BackwardLeft"; }
	if (direction == sf::Vector2i(1, 1)) { return "BackwardRight"; }
	return "None";
}


