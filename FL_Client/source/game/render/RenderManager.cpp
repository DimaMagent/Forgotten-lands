#include "pch.hpp"
#include "RenderManager.hpp"
#include "Entity.hpp"
#include "RenderComponent.hpp"
#include "TransformComponent.hpp"

RenderManager::RenderManager(sf::RenderTarget& renderTarget) :
	renderTarget(renderTarget)
{
}

void RenderManager::render(const sl::Entity& entity) const
{
	RenderComponent* rendComp = entity.getComponent<RenderComponent>();
	sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();

	if (!rendComp || !trComp) { return; }

	rendComp->render(renderTarget, trComp->getPosition());
}
