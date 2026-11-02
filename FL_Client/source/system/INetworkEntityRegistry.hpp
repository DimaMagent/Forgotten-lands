#pragma once
#include <optional>
#include <functional>

namespace sl {
	class Entity;
	struct EntityId;
}

class INetworkEntityRegistry {
public:
	virtual ~INetworkEntityRegistry() = default;
	
	virtual std::optional<std::reference_wrapper<const sl::Entity>> getEntityByNetworkId(sl::EntityId networkId) const = 0;

	virtual std::optional<std::reference_wrapper<sl::Entity>> getEntityByNetworkId(sl::EntityId networkId) = 0;

	virtual std::optional<sl::EntityId> getLocalIdByNetworkId(sl::EntityId networkId) const = 0;
};