#include "pch.h"
#include "EntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include <optional>
#include "Utils.hpp"

sl::EntityFactory::EntityFactory(std::unique_ptr<sl::DataLoader>&& dataLoader){
	this->dataLoader = std::move(dataLoader);
}

sl::EntityFactory::~EntityFactory() = default;

sl::Entity sl::EntityFactory::createEntity(sl::EntityType entityType)
{
	if (!dataLoader)
	{
		throw std::runtime_error("data loader is not valid. CreateEntity cannot be executed.");
	}

	std::optional<json> jd = dataLoader->getEntityData(entityType);

	if (!jd.has_value()) 
	{
		throw std::runtime_error("json file is not valid. CreateEntity cannot be executed.");
	}

	sl::Entity entity = sl::Entity(entityType);
	std::any services = getServiceProvider();

	for (auto& [key, value] : jd.value().items()) {
		sl::TypeID typeId = sl::fnv1a(key.c_str());
		if (getRegistry().contains(typeId)) {
			ComponentInitContext ctx{ entity, &value, services };
			getRegistry()[typeId](ctx);
		}
	}

	return entity;
}

void sl::EntityFactory::registerComponent(sl::TypeID componentTypeId, sl::ComponentFactory factory)
{
	getRegistry().try_emplace(componentTypeId, factory);
}
