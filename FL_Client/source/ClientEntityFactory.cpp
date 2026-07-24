#include "pch.hpp"
#include "ClientEntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include <nlohmann/json.hpp>
#include "RenderComponent.hpp"
#include "TextureManager.hpp"
#include "AnimationsStorage.hpp"
#include "AnimationComponent.hpp"

ClientEntityFactory::ClientEntityFactory() : sl::EntityFactory()
{
	textureManager = std::make_unique<TextureManager>();
}

ClientEntityFactory::~ClientEntityFactory() = default;

void ClientEntityFactory::registrationComponents()
{
	sl::EntityFactory::registrationComponents();

	registry.try_emplace(RenderComponent::ComponentName, [this](sl::Entity& entity, const json& js) {

		auto& rectData = js.at("textureRect");

		entity.addComponent<RenderComponent>(textureManager->getTexture(js.value("texture", "")),
			rectData.value("height", 0),
			rectData.value("width", 0),
			rectData.value("x", 0),
			rectData.value("y", 0));
	});

	registry.try_emplace(AnimationComponent::ComponentName, [this](sl::Entity& entity, const json& js) {
		std::shared_ptr<AnimationsStorage> animationStorage = std::make_shared<AnimationsStorage>();

		for (auto& [animationName, animation] : js["Animations"].items()) {
			std::vector<std::shared_ptr<sf::Texture>> directionFrames;
			for (auto& [directionName, framePaths] : animation.items()) {
				for (auto& framePath : framePaths) {
					directionFrames.push_back(textureManager->getTexture(framePath));
				}
				animationStorage->addAnimations(animationTypeFromString(animationName), directionName, directionFrames);
				directionFrames.clear();
			}
		}

		AnimationComponent& animComp = entity.addComponent<AnimationComponent>(animationStorage);

		for (auto& [animationName, frequency] : js["AllowedAnimation"].items()) {
			animComp.addAllowedAnimationFrequency(animationTypeFromString(animationName), frequency);
		}
	});

}
