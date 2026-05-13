#pragma once
#include <deque>
#include <cstdint>
#include <memory>
#include "Delegate.hpp"
#include "PacketDataTypes.hpp"

namespace sl {
	class Entity;
	namespace net {
		struct StatusData;
	}
}

//stores the sequence of player actions
class PlayerStateManager {
public:
	PlayerStateManager(sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate);
	void recordRollback(const sl::net::StatusData& data);
private:
	uint32_t lastAppliedSequence = 0;
	std::weak_ptr<sl::Entity> playerEntity;
};