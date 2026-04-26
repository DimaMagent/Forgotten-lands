#include "pch.hpp"
#include "VisualEntity.hpp"
#include "RenderComponent.hpp"
#include "TransformComponent.hpp"

VisualEntity::VisualEntity(const RenderComponent& rc, const sl::TransformComponent& tc) : sl::SimulationEntity(tc),
renderComponent(std::make_shared<RenderComponent>(rc)) {}

VisualEntity::~VisualEntity() = default;

void VisualEntity::render(sf::RenderTarget& target) const
{
	if (renderComponent && transformComponent) {
		renderComponent->Render(target, transformComponent->getPosition());
	}
}
