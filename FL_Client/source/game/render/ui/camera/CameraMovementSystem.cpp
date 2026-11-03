#include "pch.hpp"
#include "game/render/ui/camera/CameraMovementSystem.hpp"
#include "game/render/ui/camera/CameraComponent.hpp"
#include "system/systems/TransformComponent.hpp"
#include "system/Entity.hpp"
#include "system/IEntityRegistry.hpp"
#include "system/systems/ClientSystemCreatorContext.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

CameraMovementSystem::CameraMovementSystem(const sl::IEntityRegistry& entityRegistry, sf::RenderTarget& renderTarget):
	entityRegistry(entityRegistry), renderTarget(renderTarget)
{

}

void CameraMovementSystem::update(float updateTime, const sl::Entity& entity) {

	CameraComponent* cameraComp = entity.getComponent<CameraComponent>();
	if (!cameraComp) { return; }

	sl::TransformComponent* transComp = entity.getComponent<sl::TransformComponent>();
	if (!transComp) { return; }

	auto targetEntityId = cameraComp->getTargetEntityId();
	if (!targetEntityId.has_value()) { return; }

	auto targetEntity = entityRegistry.getEntityById(targetEntityId.value());
	if (!targetEntity.has_value()) { return; }

	sl::TransformComponent* targetEntityTransComp = targetEntity.value().get().getComponent<sl::TransformComponent>();
	if (!targetEntityTransComp) { return; }

	transComp->setPosition(targetEntityTransComp->getPosition());

	cameraComp->setCameraPosition(transComp->getPosition());

	renderTarget.setView(cameraComp->getCamera());
}

sl::systemPtr CameraMovementSystem::initialize(sl::SystemCreatorContext& context) {
	ClientSystemCreatorContext& clientContext = static_cast<ClientSystemCreatorContext&>(context);

	return std::make_unique<CameraMovementSystem>(clientContext.entityRegistry, clientContext.renderTarget);
}

sl::SystemProcessingPhase CameraMovementSystem::getProcessingPhase() {
	return sl::SystemProcessingPhase::Simulation;
}