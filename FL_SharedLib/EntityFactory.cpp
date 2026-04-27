#include "pch.h"
#include "EntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include <nlohmann/json.hpp>
#include "TextureManager.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "RenderComponent.hpp"

sl::EntityFactory::EntityFactory() {
	dataLoader = std::make_unique<DataLoader>();
	textureManager = std::make_unique<TextureManager>();
	InitializeCharacterIdToDataId();
	registrationComponents();
}

sl::EntityFactory::~EntityFactory() = default;

std::unique_ptr<sl::Entity> sl::EntityFactory::createEntity(const EntityType EntityId)
{
	try {
		auto it = characterIdToDataId.find(EntityId);
		if (it == characterIdToDataId.end()) {
			throw std::runtime_error("Character ID not found: " + std::to_string(static_cast<int>(EntityId)));
		}

		std::string dataId = it->second;
		json& jd = dataLoader->getData(dataId);

		std::unique_ptr<sl::Entity> entity = std::make_unique<sl::Entity>();

		for (auto& [key, value] : jd.items()) {
			if (registry.contains(key)) {
				registry[key](*entity, value);
			}
		}
		return std::move(entity);
	}
	catch (const std::exception& e) {
		std::cerr << "Error creating character: " << e.what() << std::endl;
		return nullptr;
	}
}

void sl::EntityFactory::InitializeCharacterIdToDataId()
{
	characterIdToDataId.try_emplace(EntityType::Player, "player");
}

void sl::EntityFactory::registrationComponents()
{
	registry.try_emplace("transform", [](Entity& entity, const json& js) {
		entity.addComponent<sl::TransformComponent>(js.at("position").value("x", 0.0f), js.at("position").value("y", 0.0f));
		});
	registry.try_emplace("movement", [](Entity& entity, const json& js) {
		entity.addComponent<sl::MovementComponent>(js.value("maxVelocity", 20.0f));
		});
	registry.try_emplace("render", [this](Entity& entity, const json& js) {
		auto& rectData = js.at("textureRect");
		std::shared_ptr<sf::Texture> texture = this->textureManager->getTexture(js.value("texturePath", ""));
		entity.addComponent<RenderComponent>(texture,
			rectData.value("height", 0),
			rectData.value("width", 0),
			rectData.value("x", 0),
			rectData.value("y", 0));
		});
}

