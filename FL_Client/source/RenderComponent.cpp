#include "pch.hpp"
#include "RenderComponent.hpp"
#include <SFML/Graphics.hpp>
#include "AnimationsStorage.hpp"


RenderComponent::RenderComponent(std::shared_ptr<const AnimationsStorage> animStorage, const sf::IntRect& rc) :
	animationsStorage(animStorage), rectTransform(rc)
{
	currentTexture = std::make_unique<sf::Texture>();
	animationsStorage->getAnimationFrame(AnimationType::None, "Forward", 0, *currentTexture);
}

RenderComponent::RenderComponent(std::shared_ptr<const AnimationsStorage> animStorage, int height, int width, int x, int y):
	animationsStorage(animStorage), rectTransform(sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(height, width)))
{
	currentTexture = std::make_unique<sf::Texture>();
	animationsStorage->getAnimationFrame(AnimationType::None, "Forward", 0, *currentTexture);
}

void RenderComponent::render(sf::RenderTarget& target, sf::Vector2f position) const
{
	if (currentTexture) {
		sf::Sprite sprite(*currentTexture, rectTransform);
		sprite.setPosition(position);
		target.draw(sprite);
	}
}


