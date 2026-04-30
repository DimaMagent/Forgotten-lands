#include "pch.hpp"
#include "ClientEntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include <nlohmann/json.hpp>
#include "RenderComponent.hpp"
#include "TextureManager.hpp"

ClientEntityFactory::ClientEntityFactory() : sl::EntityFactory()
{
	textureManager = std::make_unique<TextureManager>();
}

ClientEntityFactory::~ClientEntityFactory() = default;

void ClientEntityFactory::registrationComponents()
{
	sl::EntityFactory::registrationComponents();

	registry.try_emplace("render", [this](sl::Entity& entity, const json& js) {
		std::shared_ptr<sf::Texture> texture = textureManager->getTexture(js.value("texturePath", ""));
		auto& rectData = js.at("textureRect");
		entity.addComponent<RenderComponent>(texture,
			rectData.value("height", 0),
			rectData.value("width", 0),
			rectData.value("x", 0),
			rectData.value("y", 0));
	});
}
