#include "Entity.hpp"
#include "RenderComponent.hpp"

Entity::Entity(std::shared_ptr<RenderComponent> rc) : renderComponent(std::move(rc)){}

Entity::~Entity() = default;

void Entity::render(sf::RenderTarget& target) const
{
	if (renderComponent) {
		renderComponent->Render(target);
	}
}
