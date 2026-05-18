#include "pch.hpp"
#include "StateManager.hpp"
#include "StatusPacket.hpp"
#include "Entity.hpp"
#include "AuthPacket.hpp"
#include "Serializable.hpp"

StateManager::StateManager(sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate): playerEntity(playerEntity)
{
	onSetPlayerEntityDelegate.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) {this->playerEntity = playerEntity; });
}



void StateManager::recordRollback(const sl::net::StatusData& data)
{
	if (data.header.getData().sequenceNumber <= lastAppliedSequence) { return; }
	++lastAppliedSequence;
	size_t offset = 0;

	auto player = playerEntity.lock();
	if (!player) { return; }

	uint32_t entityId = 0;
	uint32_t size = 0;

	while (offset < data.data.size()) {
		if (size <= 0) {
			entityId = sl::net::read_uint32_t(data.data, offset);
			size = sl::net::read_uint32_t(data.data, offset);
		}
		uint32_t type = sl::net::read_uint32_t(data.data, offset);

		if (entityId == player->getGlobalId()) {
			std::cout << "decerialize " << entityId << player->getGlobalId() << "\n";
			player->forCurrentSerialization(type, [this, &offset, &data, &size](sl::Serializable& s) {
				s.deserialize(data.data, offset);
				size -= s.getDeserializeDataSize();
			});
		}
	}
	
}

void StateManager::auth(const sl::net::AuthData& data)
{
	auto player = playerEntity.lock();
	if (!player) { return; }
	std::cout << "auth" << "\n";
	player->setGlobalId(data.playerEntityID);
}
