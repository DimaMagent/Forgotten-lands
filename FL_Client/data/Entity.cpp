#include "pch.hpp"
#include "Entity.hpp"
#include "RenderComponent.hpp"
#include "TransformComponent.hpp"

Entity::Entity(const RenderComponent& rc, const TransformComponent& tc) : renderComponent(std::make_shared<RenderComponent>(rc)),
	transformComponent(std::make_unique<TransformComponent>(tc)) {}

Entity::~Entity() = default;

void Entity::render(sf::RenderTarget& target) const
{
	if (renderComponent && transformComponent) {
		renderComponent->Render(target, transformComponent->getPosition());
	}
}
