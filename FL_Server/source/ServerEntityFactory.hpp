#pragma once
#include "EntityFactory.hpp"

class ServerEntityFactory : public sl::EntityFactory {
public:
	ServerEntityFactory();
private:
	static const std::string PATH_TO_CHARACTERS_FILE;

	virtual void registrationComponents() override;
};