#pragma once
#include <memory>
#include "SFML/Graphics/Rect.hpp"
#include "Component.hpp"

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
	COMPONENT_TYPE(RenderComponent);
private:
	std::shared_ptr<const AnimationsStorage> animationsStorage;
	std::unique_ptr<sf::Texture> currentTexture;
	sf::IntRect rectTransform;
};