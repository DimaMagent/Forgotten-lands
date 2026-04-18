#include "RenderComponent.hpp"
#include "SFML/Graphics.hpp"

RenderComponent::RenderComponent(const sf::Texture& te, const sf::IntRect& rc) :
	texture(std::make_shared<sf::Texture>(te)), rectTransform(rc)
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


