#include "pch.hpp"
#include "AnimationFrequencyCalculatorSystem.hpp"
#include "system/Entity.hpp"
#include "system/systems/attack/WeaponComponent.hpp"
#include "system/systems/TransformComponent.hpp"
#include "game/render/animations/AnimationComponent.hpp"

AnimationFrequencyCalculatorSystem::AnimationFrequencyCalculatorSystem()
{}

void AnimationFrequencyCalculatorSystem::update(float updateTime, const sl::Entity& entity) {
	AnimationComponent* animComp = entity.getComponent<AnimationComponent>();
	if (!animComp) { return; }

	sl::TransformComponent* trComp = entity.getComponent<sl::TransformComponent>();
	if (!trComp) { return; }

	sl::WeaponComponent* weaponComp = entity.getComponent<sl::WeaponComponent>();
	if (!weaponComp) { return; }

	if(animComp->getCurrentAnimationType() == AnimationType::Attack) {
		float newFrequency = weaponComp->getAttackCooldown().count() / animComp->getFramesCountForCurrentAnimation(animComp->getCurrentAnimationType(), trComp->getRotation());

		animComp->setCurrentFrequencyOfAnimationIfExist(animComp->getCurrentAnimationType(), newFrequency);
		
	}
}

sl::systemPtr AnimationFrequencyCalculatorSystem::initialize(sl::SystemCreatorContext& context) {
	return std::make_unique<AnimationFrequencyCalculatorSystem>();
}
sl::SystemProcessingPhase AnimationFrequencyCalculatorSystem::getProcessingPhase() {
	return sl::SystemProcessingPhase::PostSimulation;
}