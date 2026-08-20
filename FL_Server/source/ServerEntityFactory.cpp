#include "pch.hpp"
#include "ServerEntityFactory.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "StateComponent.hpp"
#include "HealthComponent.hpp"
#include "CollisionComponent.hpp"
#include "WeaponComponent.hpp"
#include "Entity.hpp"
#include "ServerDataLoader.hpp"


ServerEntityFactory::ServerEntityFactory() : sl::EntityFactory(std::make_unique<ServerDataLoader>()) {
}

void ServerEntityFactory::registrationComponents()
{
	try {
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

		registry.try_emplace(sl::HealthComponent::ComponentName, [](sl::Entity& entity, const json& js) {
			entity.addComponent<sl::HealthComponent>(js.value("maxHealth", 1.0f));
			});

		registry.try_emplace(sl::WeaponComponent::ComponentName, [](sl::Entity& entity, const json& js) {
			entity.addComponent<sl::WeaponComponent>(
				js.value("attackDistance", 0.0f),
				js.value("attackDegrees", 0.0f),
				js.value("attackDamage", 0.0f)
			);
			});
	}
	catch(std::exception& e){
		std::shared_ptr<spdlog::logger> system_logger = spdlog::get("system");
		if (system_logger) {
			system_logger->error("ServerEntityFactory::registrationComponents: {}", e.what());
		}
		else {
			spdlog::error("ServerEntityFactory::registrationComponents: {}.\n Also system_logger could not be found", e.what());
		}
	}
}


