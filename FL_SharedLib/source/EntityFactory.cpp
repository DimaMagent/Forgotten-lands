#include "pch.h"
#include "EntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include "TransformComponent.hpp"
#include "MovementComponent.hpp"
#include "StateComponent.hpp"

sl::EntityFactory::EntityFactory(const std::string& pathToCharacterFile):
	PATH_TO_CHARACTERS_FILE(pathToCharacterFile){

	dataLoader = std::make_unique<sl::DataLoader>();
}

void sl::EntityFactory::initialize()
{
	registrationComponents();
}

sl::EntityFactory::~EntityFactory() = default;

std::unique_ptr<sl::Entity> sl::EntityFactory::createEntity(sl::EntityType entityType)
{
		std::string dataId = sl::EntityTypeToString(entityType);

		if (dataId == "undefined") {
			return nullptr;
		}

		json jd = dataLoader->getData(dataId, PATH_TO_CHARACTERS_FILE);

		std::unique_ptr<sl::Entity> entity = std::make_unique<sl::Entity>(entityType);

		for (auto& [key, value] : jd.items()) {
			if (registry.contains(key)) {
				registry[key](*entity, value);
			}
		}

		return std::move(entity);
}
