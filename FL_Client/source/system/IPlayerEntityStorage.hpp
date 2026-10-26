#pragma once
#include <optional>
#include <functional>
#include "system/EntityId.hpp"

namespace sl {
	class Entity;
}

class IPlayerEntityStorage {
public:
	virtual ~IPlayerEntityStorage() = default;

	virtual std::optional<std::reference_wrapper<sl::Entity>> getPlayerEntity() = 0;
	virtual std::optional<std::reference_wrapper<const sl::Entity>> getPlayerEntity() const = 0;
	virtual std::optional<sl::EntityId> getPlayerEntityId() const = 0;
};