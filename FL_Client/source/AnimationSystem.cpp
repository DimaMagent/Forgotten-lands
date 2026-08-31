#include "pch.hpp"
#include "AnimationSystem.hpp"
#include "Entity.hpp"
#include "TransformComponent.hpp"
#include "RenderComponent.hpp"
#include "WeaponComponent.hpp"
#include "StateComponent.hpp"
#include "AnimationComponent.hpp"

AnimationSystem::AnimationSystem(){}

void AnimationSystem::onUpdate(float updateTime, const sl::Entity& entity) {

	updateAnimations(entity, updateTime);
}

AnimationType AnimationSystem::selectAnimationType(const sl::StateComponent& stateComp)
{
	if (stateComp.getCurrentActionState() == sl::ActionState::MeleeAttack) { return AnimationType::Attack; }
	if (stateComp.getCurrentMovementState() == sl::MovementState::Run) { return AnimationType::Run; }
	if (stateComp.getCurrentMovementState() == sl::MovementState::Walk) { return AnimationType::Walk; }
	if (stateComp.getCurrentActionState() == sl::ActionState::Talk) { return AnimationType::Talk; }
	if (stateComp.getCurrentActionState() == sl::ActionState::Rest) { return AnimationType::Rest; }
	return AnimationType::Idle;
}

AnimationType AnimationSystem::selectInstantAnimationType(const sl::StateComponent& stateComp)
{
	if (stateComp.getCurrentLifeState() == sl::LifeState::Death) { return AnimationType::Death; }
	return AnimationType::Idle;
}

bool AnimationSystem::isInstantAnimation(AnimationType type)
{
	if (type == AnimationType::Death || type == AnimationType::Attack) { return true; }
	return false;
}

void AnimationSystem::updateAnimations(const sl::Entity& entity, float updateTime)
{
	AnimationComponent* animComp = entity.getComponent<AnimationComponent>();
	if (!animComp) { return; }

	sl::StateComponent* stateComp = entity.getComponent<sl::StateComponent>();
	if (!stateComp) { return; }

	AnimationType instantAnim = selectInstantAnimationType(*stateComp);
	AnimationType targetAnim = AnimationType::Idle;
	bool isReset = false;

	if (animComp->isAnimationPlaying()) {
		targetAnim = animComp->getCurrentAnimationType();
	}
	else if (instantAnim != AnimationType::Idle) {
		targetAnim = instantAnim;
		isReset = true;
	}
	else {
		targetAnim = selectAnimationType(*stateComp);
	}

	if (!animComp->addTimeSinceLastUpdate(targetAnim, updateTime)) {
		#ifdef DEBUG
		std::shared_ptr<spdlog::logger> game_logger = spdlog::get("game");
		game_logger->warn("AnimationSystem::updateAnimations addTimeSinceLastUpdate failed");
		#endif // DEBUG
	}

	float timeSinceLastUpdate;
	if (!animComp->getTimeSinceLastUpdate(targetAnim, timeSinceLastUpdate)) { return; }

	float currentFrequencyOfAnimation;
	if (targetAnim == AnimationType::Attack) {
		sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
		sl::WeaponComponent* weaponComp = entity.getComponent<sl::WeaponComponent>();
		currentFrequencyOfAnimation = weaponComp->getAttackCooldown().count() / animComp->getFramesCountForCurrentAnimation(targetAnim, trComp->getRotation());
	} else if (!animComp->getCurrentFrequencyOfAnimationIfExist(targetAnim, currentFrequencyOfAnimation)) { return; }

	if (timeSinceLastUpdate >= currentFrequencyOfAnimation) {

		if (!animComp->addTimeSinceLastUpdate(targetAnim, -currentFrequencyOfAnimation)) {
			std::shared_ptr<spdlog::logger> game_logger = spdlog::get("game");
			game_logger->warn("AnimationSystem::updateAnimations addTimeSinceLastUpdate failed");
		}

		sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
		RenderComponent* rendComp = entity.getComponent<RenderComponent>();

		if (!rendComp || !trComp) { return; }

		std::shared_ptr<sf::Texture> texture;

		if (isInstantAnimation(targetAnim)) {
			texture = animComp->getCurrentAnimationFramePlayingAnimation(targetAnim, trComp->getRotation(), isReset);
		}
		else {
			texture = animComp->getCurrentAnimationFrame(targetAnim, trComp->getRotation());
		}

		if (texture) {
			rendComp->setCurrentTexture(texture);
		}
	}
}
