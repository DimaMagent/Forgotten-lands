#include "pch.h"
#include "EntityType.hpp"

std::string sl::EntityTypeToString(EntityType type)
{
	if (type == EntityType::Player) { return "player"; }
	return "undefined";
}