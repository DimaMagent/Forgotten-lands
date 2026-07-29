#include "pch.h"
#include "EntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "StateComponent.hpp"
#include <optional>

sl::EntityFactory::EntityFactory(std::unique_ptr<sl::DataLoader>&& dataLoader){
	this->dataLoader = std::move(dataLoader);
}

void sl::EntityFactory::initialize()
{
	registrationComponents();
}

sl::EntityFactory::~EntityFactory() = default;

std::unique_ptr<sl::Entity> sl::EntityFactory::createEntity(sl::EntityType entityType)
{
		if (!dataLoader) { return nullptr; }

		std::optional<json> jd = dataLoader->getEntityData(entityType);

		if (!jd.has_value()) { return nullptr; }

		std::unique_ptr<sl::Entity> entity = std::make_unique<sl::Entity>(entityType);

		for (auto& [key, value] : jd.value().items()) {
			if (registry.contains(key)) {
				registry[key](*entity, value);
			}
		}

		return std::move(entity);
}
