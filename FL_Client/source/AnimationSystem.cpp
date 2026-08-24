#include "pch.hpp"
#include "AnimationSystem.hpp"
#include "Entity.hpp"
#include "TransformComponent.hpp"
#include "RenderComponent.hpp"
#include "StateComponent.hpp"
#include "AnimationComponent.hpp"

AnimationSystem::AnimationSystem(sl::EntityStorage& entities):
	entities(entities)
{
}

void AnimationSystem::onUpdate(float updateTime) {

	for (auto& en : entities.getEntities()) {
		if (!en) { continue; }

		updateAnimations(*en, updateTime);
	}
}

AnimationType AnimationSystem::selectAnimationType(const sl::StateComponent& stateComp)
{
	if (stateComp.getCurrentMovementState() == sl::MovementState::Run) { return AnimationType::Run; }
	if (stateComp.getCurrentMovementState() == sl::MovementState::Walk) { return AnimationType::Walk; }
	if (stateComp.getCurrentActionState() == sl::ActionState::Talk) { return AnimationType::Talk; }
	if (stateComp.getCurrentActionState() == sl::ActionState::Rest) { return AnimationType::Rest; }
	return AnimationType::Idle;
}

AnimationType AnimationSystem::selectInstantAnimationType(const sl::StateComponent& stateComp)
{
	if (stateComp.getCurrentLifeState() == sl::LifeState::Death) { return AnimationType::Death; }
	if (stateComp.getCurrentActionState() == sl::ActionState::MeleeAttack) { return AnimationType::Attack; }
	return AnimationType::Idle;
}

void AnimationSystem::updateAnimations(sl::Entity& entity, float updateTime)
{
	AnimationComponent* animComp = entity.getComponent<AnimationComponent>();
	if (!animComp) { return; }

	sl::StateComponent* stateComp = entity.getComponent<sl::StateComponent>();
	if (!stateComp) { return; }

	AnimationType instantAnimationType = selectInstantAnimationType(*stateComp);

	AnimationType currentAnimationType = animComp->getCurrentAnimationType();

	if (instantAnimationType != AnimationType::Idle && instantAnimationType != animComp->getCurrentAnimationType()) {
		currentAnimationType = instantAnimationType;
	} else if (!animComp->isAnimationPlaying()) {
		currentAnimationType = selectAnimationType(*stateComp);
	}


	if (!animComp->addTimeSinceLastUpdate(currentAnimationType, updateTime)) {
		std::shared_ptr<spdlog::logger> game_logger = spdlog::get("game");
		game_logger->warn("AnimationSystem::updateAnimations addTimeSinceLastUpdate failed");
	}

	float timeSinceLastUpdate;
	if (!animComp->getTimeSinceLastUpdate(currentAnimationType, timeSinceLastUpdate)) { return; }

	float currentFrequencyOfAnimation;
	if (!animComp->getCurrentFrequencyOfAnimationIfExist(currentAnimationType, currentFrequencyOfAnimation)) { return; }

	if (timeSinceLastUpdate >= currentFrequencyOfAnimation) {

		if (!animComp->addTimeSinceLastUpdate(currentAnimationType, -currentFrequencyOfAnimation)) {
			std::shared_ptr<spdlog::logger> game_logger = spdlog::get("game");
			game_logger->warn("AnimationSystem::updateAnimations addTimeSinceLastUpdate failed");
		}

		sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
		RenderComponent* rendComp = entity.getComponent<RenderComponent>();

		if (!rendComp || !trComp) { return; }

		std::shared_ptr<sf::Texture> texture;

		if (currentAnimationType == AnimationType::Attack)
		{
			
			texture = animComp->getCurrentAnimationFramePlayingAnimation(currentAnimationType, trComp->getRotation());
			if (texture)
			{
				rendComp->setCurrentTexture(texture);
				return;
			}
			else if (selectInstantAnimationType(*stateComp) == AnimationType::Attack) {

				texture = animComp->getCurrentAnimationFramePlayingAnimation(currentAnimationType, trComp->getRotation(), true);
				rendComp->setCurrentTexture(texture);
				return;
			}

			currentAnimationType = selectInstantAnimationType(*stateComp);
		}
		
		texture = animComp->getCurrentAnimationFrame(currentAnimationType, trComp->getRotation());

		if (!texture) {
			std::shared_ptr<spdlog::logger> game_logger = spdlog::get("game");
			game_logger->warn("AnimationSystem::updateAnimations animation set failed");
			return;
		}

		rendComp->setCurrentTexture(texture);
	}
}
