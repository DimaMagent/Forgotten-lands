#pragma once
#include <deque>
#include <vector>
#include <cstdint>
#include <memory>
#include "Delegate.hpp"
#include "LockFreeDelegate.hpp"
#include "PacketDataTypes.hpp"

namespace sl {
	class Entity;
	namespace net {
		struct StatusData;
		struct AuthData;
	}
}

//stores the sequence of player actions
class StateManager {
public:
	sl::LockFreeDelegate<uint32_t> OnAbsenceEntity;
	StateManager(std::weak_ptr<sl::Entity> playerEntity, std::vector<std::unique_ptr<sl::Entity>>& entities,
		sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate);
	void recordRollback(const sl::net::StatusData& data);
	void auth(const sl::net::AuthData& data);
private:
	uint32_t lastAppliedSequence = 0;
	std::weak_ptr<sl::Entity> playerEntity;
	std::vector<std::unique_ptr<sl::Entity>>& entities;
};