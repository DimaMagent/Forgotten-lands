#include "RenderComponent.hpp"
#include "SFML/Graphics.hpp"

RenderComponent::RenderComponent(const sf::Texture& te, const sf::IntRect& rc) :
	texture(std::make_shared<sf::Texture>(te)), rectTransform(rc)
{
}

void RenderComponent::Render(sf::RenderTarget& target) const
{
	if (texture) {
		target.draw(sf::Sprite(*texture, rectTransform));
	}
}

void RenderComponent::updatePosition(const sf::Vector2f& position)
{
	rectTransform.left = static_cast<int>(position.x + std::round(rectTransform.width / 2));
	rectTransform.top = static_cast<int>(position.y + std::round(rectTransform.height / 2));
}

