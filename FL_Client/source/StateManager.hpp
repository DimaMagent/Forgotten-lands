#pragma once
#include <deque>
#include <vector>
#include <cstdint>
#include <memory>
#include "Delegate.hpp"
#include "LockFreeDelegate.hpp"
#include "PacketDataTypes.hpp"
#include "EntityType.hpp"
#include "EntityId.hpp"

namespace sl {
	class Entity;
	namespace net {
		struct StatusData;
		struct AuthData;
		struct EntityData;
	}
	struct EntityStorage;

}

//stores the sequence of player actions
class StateManager {
public:
	sl::LockFreeDelegate<const sl::net::EntityData&> OnAbsenceEntity;
	sl::LockFreeDelegate<sl::EntityId> OnEntityAbsenceOnStatusPacket;
	sl::LockFreeDelegate<const sl::net::EntityData&> OnAuth;

	StateManager(sl::EntityStorage& entities);
	void recordRollback(const sl::net::StatusData& data);
	void auth(const sl::net::AuthData& data);
private:
	std::shared_ptr<spdlog::logger> net_logger;

	uint32_t lastAppliedSequence = 0;
	sl::EntityStorage& entities;
};