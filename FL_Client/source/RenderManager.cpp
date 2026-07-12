#include "pch.hpp"
#include "RenderManager.hpp"
#include "Entity.hpp"
#include "RenderComponent.hpp"
#include "TransformComponent.hpp"

RenderManager::RenderManager(sf::RenderTarget& renderTarget) :
	renderTarget(renderTarget)
{
}

void RenderManager::render(sl::Entity& entity) const
{
	RenderComponent* comp = entity.getComponent<RenderComponent>();
	sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();

	if (!comp || !trComp) { return; }

	comp->render(renderTarget, trComp->getPosition());
}
