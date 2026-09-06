#pragma once
#include <optional>
#include "EntityId.hpp"

namespace sl {
	class Entity;

	class IEntityRegistry {
	public:
		virtual ~IEntityRegistry() = default;
		virtual std::optional<std::reference_wrapper<const sl::Entity>> getEntityById(sl::EntityId id) const = 0;
		virtual std::optional<std::reference_wrapper<sl::Entity>> getEntityById(sl::EntityId id) = 0;
	};

}