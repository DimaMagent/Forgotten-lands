#include "RenderComponent.hpp"
#include "SFML/Graphics.hpp"

RenderComponent::RenderComponent(const sf::Texture& te, const sf::IntRect& rc) :
	texture(std::make_shared<sf::Texture>(te)), rect(rc)
{
}

void RenderComponent::Render(sf::RenderTarget& target) const
{
	if (texture) {
		target.draw(sf::Sprite(*texture, rect));
	}
}
