#pragma once
#include <deque>
#include <vector>
#include <cstdint>
#include <memory>
#include "system/handlers/Delegate.hpp"
#include "system/handlers/LockFreeDelegate.hpp"
#include "network/packets/PacketDataTypes.hpp"
#include "system/EntityType.hpp"
#include "system/EntityId.hpp"
#include "network/DataProcessorTypes.hpp"

namespace sl {
	class Entity;
	class IEntityStorage;
	namespace net {
		struct StatusData;
		struct AuthData;
		struct EntityData;
	}
}
class INetworkEntityRegistry;

//stores the sequence of player actions
class StateManager {
public:
	sl::LockFreeDelegate<const sl::net::EntityData&> OnAbsenceEntity;
	sl::LockFreeDelegate<sl::EntityId> OnEntityAbsenceOnStatusPacket;
	sl::LockFreeDelegate<const sl::net::EntityData&> OnAuth;

	StateManager(INetworkEntityRegistry& networkEntityRegistry, sl::IEntityStorage& entityStorage, DataProcessedDelegate& OnDataProcessed);
	void recordRollback(const sl::net::StatusData& data);
	void auth(const sl::net::AuthData& data);
private:
	std::shared_ptr<spdlog::logger> net_logger;

	uint32_t lastAppliedSequence = 0;
	sl::IEntityStorage& entityStorage;
	INetworkEntityRegistry& networkEntityRegistry;
	std::shared_ptr<sl::DelegateToken> token;
};