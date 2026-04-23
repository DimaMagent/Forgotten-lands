#include "pch.hpp"
#include "RenderComponent.hpp"
#include <SFML/Graphics/Sprite.hpp>

RenderComponent::RenderComponent(std::shared_ptr<const sf::Texture> texturePtr, const sf::IntRect& rc) :
	texture(std::move(texturePtr)), rectTransform(rc)
{
}

void RenderComponent::Render(sf::RenderTarget& target, sf::Vector2f position) const
{
	if (texture) {
		sf::Sprite sprite(*texture, rectTransform);
		sprite.setPosition(position);
		target.draw(sprite);
	}
}


