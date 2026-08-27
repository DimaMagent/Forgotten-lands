#include "pch.hpp"
#include "RenderComponent.hpp"
#include <SFML/Graphics.hpp>
#include "AnimationsStorage.hpp"
#include "Entity.hpp"
#include "TextureManager.hpp"


RenderComponent::RenderComponent(const std::shared_ptr<sf::Texture> texture, const sf::IntRect& rc) :
	rectTransform(rc)
{
	currentTexture = texture;
}

RenderComponent::RenderComponent(const std::shared_ptr<sf::Texture> texture, int height, int width, int x, int y):
	rectTransform(sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(height, width)))
{
	currentTexture = texture;
}

void RenderComponent::render(sf::RenderTarget& target, sf::Vector2f position) const
{
	if (currentTexture) {
		sf::Sprite sprite(*currentTexture, rectTransform);
		sprite.setPosition(position);
		target.draw(sprite);
	}
}

void RenderComponent::setCurrentTexture(const std::shared_ptr<sf::Texture> newTexture)
{
	if (newTexture) {
		currentTexture = newTexture;
	}
}

void RenderComponent::initialize(sl::ComponentInitContext context) {
	auto* textureManager = context.getService<TextureManager>();
	if (!textureManager) {
		throw("ComponentInitContext does not store TextureManager");
		return;
	}
	auto& rectData = context.js.at("textureRect");

	context.entity.addComponent<RenderComponent>(textureManager->getTexture(context.js.value("texture", "")),
		rectData.value("height", 0),
		rectData.value("width", 0),
		rectData.value("x", 0),
		rectData.value("y", 0));
}

