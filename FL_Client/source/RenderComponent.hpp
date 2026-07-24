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

	RenderComponent(const std::shared_ptr<sf::Texture> texture, const sf::IntRect& rc);

	RenderComponent(const std::shared_ptr<sf::Texture> texture, int height, int width, int x, int y);

	void render(sf::RenderTarget& target, sf::Vector2f position) const;

	void setCurrentTexture(const std::shared_ptr<sf::Texture> newTexture);

	COMPONENT_TYPE(RenderComponent);
private:

	std::shared_ptr<sf::Texture> currentTexture;

	sf::IntRect rectTransform;

};