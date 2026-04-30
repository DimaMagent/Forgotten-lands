#pragma once
#include "EntityFactory.hpp"

class TextureManager;


class ClientEntityFactory: public sl::EntityFactory {
public:
	ClientEntityFactory();
	~ClientEntityFactory();
protected:
	std::unique_ptr<TextureManager> textureManager;

	virtual void registrationComponents() override;
};