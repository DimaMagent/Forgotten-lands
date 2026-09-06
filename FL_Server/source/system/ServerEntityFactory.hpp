#pragma once
#include "system/EntityFactory.hpp"

class ServerEntityFactory : public sl::EntityFactory {
public:
	ServerEntityFactory();
private:
	static const std::string PATH_TO_CHARACTERS_FILE;
};