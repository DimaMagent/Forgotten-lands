#pragma once
#include "EntityFactory.hpp"

class TextureManager;
namespace sl {
	namespace net {
		struct EntityData;
	}
	class Serializable;
}

class ClientEntityFactory: public sl::EntityFactory {
public:
	ClientEntityFactory();
	~ClientEntityFactory();
	std::unique_ptr<sl::Entity> entityCollection(const sl::net::EntityData& enData);
protected:
	std::unique_ptr<TextureManager> textureManager;

	virtual std::any getServiceProvider() override;
	sl::Serializable* createAndAttach(uint32_t typeId, sl::Entity& entity);
};