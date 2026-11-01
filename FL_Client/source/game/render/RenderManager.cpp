#include "pch.hpp"
#include "RenderManager.hpp"
#include "system/Entity.hpp"
#include "RenderComponent.hpp"
#include "system/systems/TransformComponent.hpp"
#include "game/render/CameraComponent.hpp"

RenderManager::RenderManager(sf::RenderTarget& renderTarget) :
	renderTarget(renderTarget)
{
}

void RenderManager::render(const sl::Entity& entity) const
{
	RenderComponent* rendComp = entity.getComponent<RenderComponent>();
	if (!rendComp) { return; }

	sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
	if (!trComp) { return; }
	
	/*
	CameraComponent* cameraComp = entity.getComponent<CameraComponent>();
	if (cameraComp) {
		cameraComp->setCameraPosition(trComp->getPosition());
		renderTarget.setView(cameraComp->getCamera());
		std::cout << "camera set on entity with id: " << entity.getId().getIndex() << "\n";
		
	}
	*/


	rendComp->render(renderTarget, trComp->getPosition());
}
