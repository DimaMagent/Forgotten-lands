#include "pch.hpp"
#include "PlayerStateManager.hpp"
#include "StatusPacket.hpp"
#include "Entity.hpp"
#include "Serializable.hpp"

PlayerStateManager::PlayerStateManager(sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate): playerEntity(playerEntity)
{
	onSetPlayerEntityDelegate.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) {this->playerEntity = playerEntity; });
}



void PlayerStateManager::recordRollback(const sl::net::StatusData& data)
{
	if (data.header.getData().sequenceNumber <= lastAppliedSequence) { return; }
	++lastAppliedSequence;
	size_t offset = 0;

	auto player = playerEntity.lock();
	if (!player) { return; }
	
	while (offset <= data.data.size() - 1) {
		uint32_t size = sl::net::read_uint32_t(data.data, offset);
		uint32_t type = sl::net::read_uint32_t(data.data, offset);
		player->forCurrentSerialization(type, [this, &offset, &data](sl::Serializable& s) {
			s.deserialize(data.data, offset);
		});
	}

	//std::cout << "Received server data: " << x << " ; " << y << "\n";
	
}
