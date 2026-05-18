#include "pch.hpp"
#include "RenderManager.hpp"
#include "Entity.hpp"
#include "RenderComponent.hpp"
#include "TransformComponent.hpp"

RenderManager::RenderManager(sf::RenderTarget& renderTarget, std::weak_ptr<sl::Entity> playerEntity,
	std::vector<std::unique_ptr<sl::Entity>>& entities, sl::Delegate<const std::weak_ptr<sl::Entity>>& OnSetPlayerEntity) :
	renderTarget(renderTarget), playerEntity(playerEntity), entities(entities)
{
	OnSetPlayerEntity.addFunction([this](const std::weak_ptr<sl::Entity> pe) { this->playerEntity = pe; });
}

void RenderManager::render() const
{
	if (!entities.empty()) {
		for (const auto& entity : entities) {
			if (!entity) { continue; }

			RenderComponent* comp = entity->getComponent<RenderComponent>();
			sl::TransformComponent* trComp = entity->getComponent<sl::TransformComponent>();

			if (!comp || !trComp) { continue; }

			comp->render(renderTarget, trComp->getPosition());

		}
	}
	auto pe = playerEntity.lock();
	if (pe) {
		RenderComponent* comp = pe->getComponent<RenderComponent>();
		sl::TransformComponent* trComp = pe->getComponent<sl::TransformComponent>();
		if (comp && trComp) {
			comp->render(renderTarget, trComp->getPosition());
		}
	}
}
