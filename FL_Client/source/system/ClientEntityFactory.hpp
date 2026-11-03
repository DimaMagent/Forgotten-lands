#pragma once
#include "system/EntityFactory.hpp"

class TextureManager;
class IPlayerEntityStorage;
struct ClientComponentInitContext;
namespace sl {
	namespace net {
		struct EntityData;
	}
	class Serializable;
}
namespace sf {
	class RenderTarget;
}

class ClientEntityFactory: public sl::EntityFactory {
public:
	ClientEntityFactory(const IPlayerEntityStorage& playerEntityStorage);
	~ClientEntityFactory();
	sl::Entity entityCollection(const sl::net::EntityData& enData);
protected:
	std::unique_ptr<TextureManager> textureManager;
	std::unique_ptr<ClientComponentInitContext> clientContext;

	virtual std::any getServiceProvider() override;
	sl::Serializable* createAndAttach(uint32_t typeId, sl::Entity& entity);
};