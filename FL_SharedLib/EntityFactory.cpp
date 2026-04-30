#include "pch.h"
#include "EntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"

sl::EntityFactory::EntityFactory() {
	
	dataLoader = std::make_unique<DataLoader>();
	InitializeCharacterIdToDataId();
}

void sl::EntityFactory::initialize()
{
	registrationComponents();
}

sl::EntityFactory::~EntityFactory() = default;

std::unique_ptr<sl::Entity> sl::EntityFactory::createEntity(const EntityType EntityId)
{
		auto it = characterIdToDataId.find(EntityId);
		if (it == characterIdToDataId.end()) {
			return nullptr;
		}

		std::string dataId = it->second;
		json jd = dataLoader->getData(dataId);

		std::unique_ptr<sl::Entity> entity = std::make_unique<sl::Entity>();

		for (auto& [key, value] : jd.items()) {
			if (registry.contains(key)) {
				registry[key](*entity, value);
			}
			else {
				std::cout << "Registry have not: " << key << "\n";
			}
		}
		return entity;
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

}

