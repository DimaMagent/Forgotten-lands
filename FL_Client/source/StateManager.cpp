#include "pch.hpp"
#include "StateManager.hpp"
#include "StatusPacket.hpp"
#include "Entity.hpp"
#include "AuthPacket.hpp"
#include "Serializable.hpp"
#include "EntityStorage.hpp"

StateManager::StateManager(std::weak_ptr<sl::Entity> playerEntity, sl::EntityStorage& entities,
		sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate): playerEntity(playerEntity), entities(entities)
{
	net_logger = spdlog::get("network");
	onSetPlayerEntityDelegate.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) {this->playerEntity = playerEntity; });
}



void StateManager::recordRollback(const sl::net::StatusData& data)
{
	if (data.header.getData().sequenceNumber <= lastAppliedSequence) { return; }
	++lastAppliedSequence;
	size_t offset = 0;

	auto player = playerEntity.lock();
	if (!player) { return; }

	std::vector<sl::net::EntityData> entityData = data.getEntityData();

	std::vector<uint32_t> typeIds;

	for (auto& enData : entityData) {
		typeIds.push_back(enData.entityId);
		for (auto& compData : enData.componentsData) {

			if (enData.entityId == player->getGlobalId()) 
			{
				player->forCurrentSerialization(compData.typeId, [this, &compData](sl::Serializable& s) 
					{
					size_t offset = 0;
					s.deserialize(compData.componentData, offset);
					});
			}
			else if (auto en = entities.getEntityToId(enData.entityId).lock(); en)
			{
				en->forCurrentSerialization(compData.typeId, [this, &compData](sl::Serializable& s) {
					size_t offset = 0;
					s.deserialize(compData.componentData, offset);
					});
			}
			else 
			{
				OnAbsenceEntity.broadcast(enData);
			}
		}
		
	}
	for (size_t i = 0; entities.getEntities().size() > i; ++i) {
		auto it = std::find(typeIds.begin(), typeIds.end(), entities.getEntities()[i]->getGlobalId());
		if (it == typeIds.end()) {
			OnEntityAbsenceOnStatusPacket.broadcast(entities.getEntities()[i]->getGlobalId());
		}
	}
}

void StateManager::auth(const sl::net::AuthData& data)
{
	net_logger->info("Player authenticated with global ID: {}", data.playerEntityID);
	std::vector<sl::net::EntityData> statData = data.getEntityData();
	if (statData.size() > 0) {
		OnAuth.broadcast(statData[0]);
	}
	else {
		net_logger->error("StateManager::auth: statusData for PlayerEntity is Empty");
	}
}
