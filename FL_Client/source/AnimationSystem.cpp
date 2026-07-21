#include "pch.hpp"
#include "AnimationSystem.hpp"
#include "Entity.hpp"
#include "TransformComponent.hpp"
#include "RenderComponent.hpp"
#include "StateComponent.hpp"


int AnimationSystem::serializationFrequency = 5;

AnimationSystem::AnimationSystem(std::weak_ptr<sl::Entity> playerEntity, sl::EntityStorage& entities,
	sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate): playerEntity(playerEntity), entities(entities)
{

	onSetPlayerEntityDelegate.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) {this->playerEntity = playerEntity; });
}

void AnimationSystem::onUpdate(float updateTime) {
	if (serializationFrequency <= ++serializationCounter) {
		serializationCounter = 0;

		auto plEn = playerEntity.lock();

		if (plEn) {
			updateAnimations(*plEn, updateTime);
		}

		for (auto& en : entities.getEntities()) {
			if (!en) { continue; }

			updateAnimations(*en, updateTime);
		}
	}
}

AnimationType AnimationSystem::selectAnimationType(const sl::StateComponent& stateComp)
{
	if (stateComp.lifeState == sl::LifeState::Death) { return AnimationType::Death; }
	if (stateComp.actionState == sl::ActionState::MeleeAttack) { return AnimationType::Attack; }
	if (stateComp.movementState == sl::MovementState::Run) { return AnimationType::Run; }
	if (stateComp.movementState == sl::MovementState::Walk) { return AnimationType::Walk; }
	if (stateComp.lifeState == sl::LifeState::TakingDamage) { return AnimationType::TakingDamage; }
	if (stateComp.actionState == sl::ActionState::Talk) { return AnimationType::Talk; }
	if (stateComp.actionState == sl::ActionState::Rest) { return AnimationType::Rest; }
	return AnimationType::Idle;
}

void AnimationSystem::updateAnimations(sl::Entity& entity, float updateTime)
{
	sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
	sl::StateComponent* stateComp = entity.getComponent<sl::StateComponent>();
	RenderComponent* rendComp = entity.getComponent<RenderComponent>();

	if (!rendComp || !trComp || !stateComp) { return; }

	bool isSuc = rendComp->setCurrentAnimation(selectAnimationType(*stateComp), trComp->getRotation());

	if (!isSuc) {
		std::shared_ptr<spdlog::logger> game_logger = spdlog::get("game");
		game_logger->warn("AnimationSystem::onUpdate animation set failed");
	}
}
