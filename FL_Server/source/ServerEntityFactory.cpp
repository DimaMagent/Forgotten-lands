#include "pch.hpp"
#include "ServerEntityFactory.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "StateComponent.hpp"
#include "CollisionComponent.hpp"
#include "Entity.hpp"
#include "ServerDataLoader.hpp"


ServerEntityFactory::ServerEntityFactory() : sl::EntityFactory(std::make_unique<ServerDataLoader>()) {
}

void ServerEntityFactory::registrationComponents()
{
	registry.try_emplace(sl::TransformComponent::ComponentName, [](sl::Entity& entity, const json& js) {
		entity.addComponent<sl::TransformComponent>(js.at("position").value("x", 0.0f), js.at("position").value("y", 0.0f));
		});
	registry.try_emplace(sl::MovementComponent::ComponentName, [](sl::Entity& entity, const json& js) {
		entity.addComponent<sl::MovementComponent>(js.value("maxVelocity", 20.0f));
		});
	registry.try_emplace(sl::StateComponent::ComponentName, [](sl::Entity& entity, const json& js) {
		entity.addComponent<sl::StateComponent>();
		});
	registry.try_emplace(sl::CollisionComponent::ComponentName, [](sl::Entity& entity, const json& js) {
		entity.addComponent<sl::CollisionComponent>(
			js.value("width", 96.0f),
			js.value("height", 96.0f),
			js.value("isStatic", true),
			sl::stringToCollisionType(js.value("CollisionType", "None"))
			);
		});

}


