#include "pch.hpp"
#include "ClientEntityFactory.hpp"
#include "system/Entity.hpp"
#include "system/DataLoader.hpp"
#include <nlohmann/json.hpp>
#include "network/packets/StatusPacket.hpp"
#include "game/render/TextureManager.hpp"
#include "ClientDataLoader.hpp"
#include "network/Serializable.hpp"
#include "ClientComponentInitContext.hpp"

ClientEntityFactory::ClientEntityFactory(const IPlayerEntityStorage& playerEntityStorage) :
	sl::EntityFactory(std::make_unique<ClientDataLoader>())
{
	textureManager = std::make_unique<TextureManager>();

	clientContext = std::make_unique<ClientComponentInitContext>(*textureManager.get(), playerEntityStorage);
}

ClientEntityFactory::~ClientEntityFactory() = default;

sl::Entity ClientEntityFactory::entityCollection(const sl::net::EntityData& enData)
{
	sl::entityType type = enData.entityType;

	sl::Entity entity = createEntity(type);

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
	return clientContext.get();
}

sl::Serializable* ClientEntityFactory::createAndAttach(uint32_t typeId, sl::Entity& entity) {
	auto it = getRegistry().find(typeId);
	if (it != getRegistry().end()) {
		sl::Component* comp = entity.getComponent(typeId);
		if (!comp) {
			sl::ComponentInitContext ctx{ entity, nullptr, nullptr};
			it->second(ctx);
		}
		return dynamic_cast<sl::Serializable*>(comp);
	}

	return nullptr;
}
