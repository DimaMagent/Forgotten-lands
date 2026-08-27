#include "pch.hpp"
#include "ClientEntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include <nlohmann/json.hpp>
#include "NetworkComponentRegistry.hpp"
#include "StatusPacket.hpp"
#include "TextureManager.hpp"
#include "ClientDataLoader.hpp"

ClientEntityFactory::ClientEntityFactory() : sl::EntityFactory(std::make_unique<ClientDataLoader>())
{
	textureManager = std::make_unique<TextureManager>();
	networkComponentRegistry = std::make_unique<NetworkComponentRegistry>();
}

ClientEntityFactory::~ClientEntityFactory() = default;

std::unique_ptr<sl::Entity> ClientEntityFactory::entityCollection(const sl::net::EntityData& enData)
{
	try {
		uint32_t type = enData.entityType;

		std::unique_ptr<sl::Entity> entity = createEntity(static_cast<sl::EntityType>(type));
		entity->setGlobalId(enData.entityId);

		for (auto& compData : enData.componentsData) {
			networkComponentRegistry->createAndAttach(compData.typeId, *entity);
			entity->forCurrentSerialization(compData.typeId, [&compData](sl::Serializable& s) {
				size_t offset = 0;
				s.deserialize(compData.componentData, offset);
				});
		}
		return entity;
	}
	catch (std::exception& e) {
		spdlog::get("network")->error("ClientEntityFactory::entityCollection exception {}", e.what());
	}
}

std::any ClientEntityFactory::getServiceProvider()
{
	return textureManager.get();
}
