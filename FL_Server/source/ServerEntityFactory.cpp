#include "pch.hpp"
#include "ServerEntityFactory.hpp"
#include "Entity.hpp"
#include "ServerDataLoader.hpp"


ServerEntityFactory::ServerEntityFactory() : sl::EntityFactory(std::make_unique<ServerDataLoader>()) {}


