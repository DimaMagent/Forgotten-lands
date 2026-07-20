#pragma once
#include <memory>
#include "SFML/Graphics/Rect.hpp"
#include "Component.hpp"
#include "AnimationType.hpp"

namespace sf {
	class Texture;
	class RenderTarget;
}

class AnimationsStorage;


class RenderComponent: public sl::Component {
public:
	RenderComponent() = default;
	RenderComponent(std::shared_ptr<const AnimationsStorage> animStorage, const sf::IntRect& rc);
	RenderComponent(std::shared_ptr<const AnimationsStorage> animStorage, int height, int width, int x, int y);
	void render(sf::RenderTarget& target, sf::Vector2f position) const;

	//returns true if animation exist
	bool setCurrentAnimation(AnimationType type, const sf::Vector2i& direction);

	COMPONENT_TYPE(RenderComponent);
private:
	std::shared_ptr<const AnimationsStorage> animationsStorage;
	std::unique_ptr<sf::Texture> currentTexture;
	AnimationType currentAnimationType;
	sf::Vector2i currentDirection;
	size_t currentIndex = 0;

	sf::IntRect rectTransform;

};