#include "pch.h"
#include "EntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include <optional>

sl::EntityFactory::EntityFactory(std::unique_ptr<sl::DataLoader>&& dataLoader){
	this->dataLoader = std::move(dataLoader);
}

sl::EntityFactory::~EntityFactory() = default;

std::unique_ptr<sl::Entity> sl::EntityFactory::createEntity(sl::EntityType entityType)
{
	try {
		if (!dataLoader) { return nullptr; }

		std::optional<json> jd = dataLoader->getEntityData(entityType);

		if (!jd.has_value()) { return nullptr; }

		std::unique_ptr<sl::Entity> entity = std::make_unique<sl::Entity>(entityType);
		std::any services = getServiceProvider();

		for (auto& [key, value] : jd.value().items()) {
			if (getRegistry().contains(key)) {
				ComponentInitContext ctx{ *entity, value, services };
				getRegistry()[key](ctx);
			}
		}

		return std::move(entity);
	}
	catch (std::exception& e) {
		std::cerr << "EntityFactory::createEntity: " << e.what() << "\n";
	}
}

void sl::EntityFactory::registerComponent(std::string_view componentName, sl::ComponentFactory factory)
{
	getRegistry().try_emplace(componentName, factory);
}
