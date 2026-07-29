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
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "StateComponent.hpp"
#include "ClientDataLoader.hpp"

ClientEntityFactory::ClientEntityFactory() : sl::EntityFactory(std::make_unique<ClientDataLoader>())
{
	textureManager = std::make_unique<TextureManager>();
	networkComponentRegistry = std::make_unique<NetworkComponentRegistry>();
}

ClientEntityFactory::~ClientEntityFactory() = default;

std::unique_ptr<sl::Entity> ClientEntityFactory::entityCollection(const sl::net::EntityData& enData)
{
	uint32_t type = enData.entityType;

	std::unique_ptr<sl::Entity> entity = createEntity(static_cast<sl::EntityType>(type));
	entity->setGlobalId(enData.entityId);

	for (auto& compData : enData.componentsData) {
		networkComponentRegistry->createAndAttach(compData.typeId, *entity);
		entity->forCurrentSerialization(compData.typeId, [&compData](sl::Serializable& s) {
			size_t offset = 0;
			s.deserialize(compData.componentData, offset);
		});
	}
	return entity;
}

void ClientEntityFactory::registrationComponents()
{

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

		if (js.contains("Animations") && js["Animations"].is_object()) {
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
		}

		AnimationComponent& animComp = entity.addComponent<AnimationComponent>(animationStorage);

		if (js.contains("AllowedAnimation") && js["AllowedAnimation"].is_object()) {
			for (auto& [animationName, frequency] : js["AllowedAnimation"].items()) {
				animComp.addAllowedAnimationFrequency(animationTypeFromString(animationName), frequency);
			}
		}
	});
}
