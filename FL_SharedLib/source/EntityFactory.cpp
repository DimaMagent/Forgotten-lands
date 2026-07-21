#include "pch.h"
#include "EntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "StateComponent.hpp"

sl::EntityFactory::EntityFactory() {
	
	dataLoader = std::make_unique<DataLoader>();
}

void sl::EntityFactory::initialize()
{
	registrationComponents();
}

sl::EntityFactory::~EntityFactory() = default;

std::unique_ptr<sl::Entity> sl::EntityFactory::createEntity(const EntityType EntityId)
{
		std::string dataId = EntityTypeToString(EntityId);

		if (dataId == "undefined") {
			return nullptr;
		}

		json jd = dataLoader->getData(dataId);

		std::unique_ptr<sl::Entity> entity = std::make_unique<sl::Entity>();

		for (auto& [key, value] : jd.items()) {
			if (registry.contains(key)) {
				registry[key](*entity, value);
			}
		}

		return entity;
}

void sl::EntityFactory::registrationComponents()
{
	registry.try_emplace(sl::TransformComponent::ComponentName, [](Entity& entity, const json& js) {
		entity.addComponent<sl::TransformComponent>(js.at("position").value("x", 0.0f), js.at("position").value("y", 0.0f));
		});
	registry.try_emplace(sl::MovementComponent::ComponentName, [](Entity& entity, const json& js) {
		entity.addComponent<sl::MovementComponent>(js.value("maxVelocity", 20.0f));
		});
	registry.try_emplace(sl::StateComponent::ComponentName, [](Entity& entity, const json& js) {
		entity.addComponent<sl::StateComponent>();
		});

}

std::string sl::EntityTypeToString(EntityType type)
{
	if (type == EntityType::Player) { return "player"; }
	return "undefined";
}
