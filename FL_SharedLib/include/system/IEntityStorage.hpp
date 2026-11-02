#pragma once
#include <span>

namespace sl {
	class Entity;

	class IEntityStorage {
	public:
		virtual ~IEntityStorage() = default;

		virtual std::span<sl::Entity> getEntities() = 0;

		virtual std::span<const sl::Entity> getEntities() const = 0;
	};
}