#pragma once
#include <memory>
#include "SFML/Graphics/Rect.hpp"
#include "Component.hpp"

namespace sf {
	class Texture;
	class RenderTarget;
}


class RenderComponent: public sl::Component {
public:
	RenderComponent() = default;
	RenderComponent(std::shared_ptr<const sf::Texture> texturePtr, const sf::IntRect& rc);
	RenderComponent(std::shared_ptr<const sf::Texture> texturePtr, int height, int width, int x, int y);
	void render(sf::RenderTarget& target, sf::Vector2f position) const;
private:
	std::shared_ptr<const sf::Texture> texture;
	sf::IntRect rectTransform;
};