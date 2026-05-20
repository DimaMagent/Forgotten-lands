#include "pch.hpp"
#include "StateManager.hpp"
#include "StatusPacket.hpp"
#include "Entity.hpp"
#include "AuthPacket.hpp"
#include "Serializable.hpp"

StateManager::StateManager(std::weak_ptr<sl::Entity> playerEntity, std::vector<std::unique_ptr<sl::Entity>>& entities,
		sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate): playerEntity(playerEntity), entities(entities)
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

	sl::net::StatusPacket packet;
	packet.readData(data);

	std::vector<sl::net::EntityData> entityData = packet.getData().getEntityData();

	for (auto& enData : entityData) {
		for (auto& compData : enData.componentsData) {

			if (enData.entityId == player->getGlobalId()) 
			{
				player->forCurrentSerialization(compData.typeId, [this, &compData](sl::Serializable& s) 
					{
					size_t offset = 0;
					s.deserialize(compData.componentData, offset);
					});
			}
			else if (auto it = std::lower_bound(entities.begin(), entities.end(), enData.entityId,
				[](std::unique_ptr<sl::Entity>& entity, uint32_t id) { return entity->getGlobalId() < id; }); it != entities.end())
			{
				(*it)->forCurrentSerialization(compData.typeId, [this, &compData](sl::Serializable& s) {
					size_t offset = 0;
					s.deserialize(compData.componentData, offset);
					});
			}
			else 
			{
				OnAbsenceEntity.broadcast(enData.entityId);
			}
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
