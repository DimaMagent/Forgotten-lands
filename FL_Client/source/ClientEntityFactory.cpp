#include "pch.hpp"
#include "ClientEntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include <nlohmann/json.hpp>
#include "RenderComponent.hpp"
#include "TextureManager.hpp"
#include "AnimationsStorage.hpp"
#include "AnimationComponent.hpp"
#include "NetworkComponentRegistry.hpp"
#include "StatusPacket.hpp"

ClientEntityFactory::ClientEntityFactory() : sl::EntityFactory()
{
	textureManager = std::make_unique<TextureManager>();
	networkComponentRegistry = std::make_unique<NetworkComponentRegistry>();
}

ClientEntityFactory::~ClientEntityFactory() = default;

std::unique_ptr<sl::Entity> ClientEntityFactory::entityCollection(const sl::net::EntityData& enData)
{
	std::unique_ptr<sl::Entity> entity = std::make_unique<sl::Entity>(enData.entityType);
	entity->setGlobalId(enData.entityId);

	for (auto& compData : enData.componentsData) {
		networkComponentRegistry->createAndAttach(compData.typeId, *entity);
		entity->forCurrentSerialization(compData.typeId, [this, &compData](sl::Serializable& s) {
			size_t offset = 0;
			s.deserialize(compData.componentData, offset);
		});
	}
	
	return std::unique_ptr<sl::Entity>();
}

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
