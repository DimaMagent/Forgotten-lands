#pragma once
#include <memory>
#include "SFML/Graphics/Rect.hpp"

namespace sf {
	class Texture;
	class RenderTarget;
}


class RenderComponent {
public:
	RenderComponent() = default;
	RenderComponent(const sf::Texture& texture, const sf::IntRect& rc);
	void Render(sf::RenderTarget& target, sf::Vector2f position) const;
private:
	std::shared_ptr<sf::Texture> texture;
	sf::IntRect rectTransform;
};