#include "pch.hpp"
#include "ClientEntityFactory.hpp"
#include "Entity.hpp"
#include "DataLoader.hpp"
#include <nlohmann/json.hpp>
#include "StatusPacket.hpp"
#include "TextureManager.hpp"
#include "ClientDataLoader.hpp"
#include "Serializable.hpp"

ClientEntityFactory::ClientEntityFactory() : sl::EntityFactory(std::make_unique<ClientDataLoader>())
{
	textureManager = std::make_unique<TextureManager>();
}

ClientEntityFactory::~ClientEntityFactory() = default;

sl::Entity ClientEntityFactory::entityCollection(const sl::net::EntityData& enData)
{
	uint32_t type = enData.entityType;

	sl::Entity entity = createEntity(static_cast<sl::EntityType>(type));

	for (auto& compData : enData.componentsData) {
		sl::Serializable* s = createAndAttach(compData.typeId, entity);
		if (!s) { continue; }

		size_t offset = 0;
		s->deserialize(compData.componentData, offset);
	}
	return entity;

}

std::any ClientEntityFactory::getServiceProvider()
{
	return textureManager.get();
}

sl::Serializable* ClientEntityFactory::createAndAttach(uint32_t typeId, sl::Entity& entity) {
	auto it = getRegistry().find(typeId);
	if (it != getRegistry().end()) {
		sl::Component* comp = entity.getComponent(typeId);
		if (!comp) {
			sl::ComponentInitContext ctx{ entity, nullptr, nullptr };
			it->second(ctx);
		}
		return dynamic_cast<sl::Serializable*>(comp);
	}

	return nullptr;
}
