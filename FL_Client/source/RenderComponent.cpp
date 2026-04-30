#include "pch.hpp"
#include "RenderComponent.hpp"
#include <SFML/Graphics.hpp>

RenderComponent::RenderComponent(std::shared_ptr<const sf::Texture> texturePtr, const sf::IntRect& rc) :
	texture(texturePtr), rectTransform(rc)
{
}

RenderComponent::RenderComponent(std::shared_ptr<const sf::Texture> texturePtr, int height, int width, int x, int y):
	texture(texturePtr), rectTransform(sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(height, width)))
{
}

void RenderComponent::render(sf::RenderTarget& target, sf::Vector2f position) const
{
	if (texture) {
		sf::Sprite sprite(*texture, rectTransform);
		sprite.setPosition(position);
		target.draw(sprite);
	}
}


