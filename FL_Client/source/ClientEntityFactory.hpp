#pragma once
#include "EntityFactory.hpp"

class TextureManager;
class NetworkComponentRegistry;
namespace sl {
	namespace net {
		struct EntityData;
	}
}

class ClientEntityFactory: public sl::EntityFactory {
public:
	ClientEntityFactory();
	~ClientEntityFactory();
	std::unique_ptr<sl::Entity> entityCollection(const sl::net::EntityData& enData);
protected:
	std::unique_ptr<TextureManager> textureManager;
	std::unique_ptr<NetworkComponentRegistry> networkComponentRegistry;

	virtual std::any getServiceProvider() override;
};