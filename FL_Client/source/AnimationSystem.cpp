#include "pch.hpp"
#include "AnimationSystem.hpp"
#include "Entity.hpp"
#include "TransformComponent.hpp"
#include "RenderComponent.hpp"
#include "StateComponent.hpp"
#include "AnimationComponent.hpp"

AnimationSystem::AnimationSystem(std::weak_ptr<sl::Entity> playerEntity, sl::EntityStorage& entities,
	sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate): playerEntity(playerEntity), entities(entities)
{

	onSetPlayerEntityDelegate.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) {this->playerEntity = playerEntity; });
}

void AnimationSystem::onUpdate(float updateTime) {
	auto plEn = playerEntity.lock();

	if (plEn) {
		updateAnimations(*plEn, updateTime);
	}

	for (auto& en : entities.getEntities()) {
		if (!en) { continue; }

		updateAnimations(*en, updateTime);
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
	AnimationComponent* animComp = entity.getComponent<AnimationComponent>();
	if (!animComp) { return; }

	sl::StateComponent* stateComp = entity.getComponent<sl::StateComponent>();
	if (!stateComp) { return; }

	AnimationType currentAnimationType = selectAnimationType(*stateComp);

	animComp->addTimeSinceLastUpdate(currentAnimationType, updateTime);

	float timeSinceLastUpdate;
	if (!animComp->getTimeSinceLastUpdate(currentAnimationType, timeSinceLastUpdate)) { return; }

	float currentFrequencyOfAnimation;
	if (!animComp->getCurrentFrequencyOfAnimationIfExist(currentAnimationType, currentFrequencyOfAnimation)) { return; }

	if (timeSinceLastUpdate >= currentFrequencyOfAnimation) {
		animComp->addTimeSinceLastUpdate(currentAnimationType, -currentFrequencyOfAnimation);

		sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
		RenderComponent* rendComp = entity.getComponent<RenderComponent>();


		if (!rendComp || !trComp) { return; }

		const std::shared_ptr<sf::Texture> texture = animComp->getCurrentAnimationFrame(currentAnimationType, trComp->getRotation());

		if (!texture) {
			std::shared_ptr<spdlog::logger> game_logger = spdlog::get("game");
			game_logger->warn("AnimationSystem::onUpdate animation set failed");
			return;
		}

		rendComp->setCurrentTexture(texture);
	}
}
