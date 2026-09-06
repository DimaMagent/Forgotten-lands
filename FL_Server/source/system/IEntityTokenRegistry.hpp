#pragma once
#include <optional>
#include <cstdint>
#include "EntityId.hpp"

namespace sl {
	class Entity;
}

class IEntityTokenRegistry {
public:
	virtual ~IEntityTokenRegistry() = default;

	virtual std::optional<std::reference_wrapper<const sl::Entity>> getPlayerEntityByToken(uint32_t token) const = 0;
	virtual std::optional<std::reference_wrapper<sl::Entity>> getPlayerEntityByToken(uint32_t token) = 0;
	virtual std::optional<uint32_t> getTokenById(sl::EntityId id) const = 0;
};