#include "pch.hpp"
#include "StateManager.hpp"
#include "network/packets/StatusPacket.hpp"
#include "system/Entity.hpp"
#include "network/packets/AuthPacket.hpp"
#include "network/Serializable.hpp"
#include "system/storages/EntityStorage.hpp"
#include "network/packets/PacketManager.hpp"
#include "system/INetworkEntityRegistry.hpp"
#include "system/IEntityStorage.hpp"

StateManager::StateManager(INetworkEntityRegistry& networkEntityRegistry, sl::IEntityStorage& entityStorage, DataProcessedDelegate& OnDataProcessed):
	networkEntityRegistry(networkEntityRegistry), entityStorage(entityStorage)
{
	net_logger = spdlog::get("network");
	token = OnDataProcessed.addFunction([this](sl::net::PacketType type, std::vector<uint8_t>&& data) {
		switch (type) {
		case sl::net::PacketType::PT_Status:
		{
			sl::net::StatusPacket pkt;
			sl::net::PacketManager::read(data, pkt);
			recordRollback(pkt.getData());
			break;
		}
		case sl::net::PacketType::PT_Auth:
		{
			sl::net::AuthPacket pkt;
			sl::net::PacketManager::read(data, pkt);
			auth(pkt.getData());
			break;
		}
		default:
			net_logger->warn("StateManager: Unhandled packet type: {}", static_cast<uint8_t>(type));
			break;
		}
		});	
}

void StateManager::recordRollback(const sl::net::StatusData& data)
{
	if (data.header.getData().sequenceNumber <= lastAppliedSequence) { return; }
	++lastAppliedSequence;
	size_t offset = 0;

	std::vector<sl::net::EntityData> entityData = data.getEntityData();

	std::vector<uint32_t> typeIds;

	for (auto& enData : entityData) {
		typeIds.push_back(enData.entityId);
		for (auto& compData : enData.componentsData) {

			if (auto en = networkEntityRegistry.getEntityByNetworkId(enData.entityId); en)
			{
				if (!en.has_value()) {
					net_logger->warn("StateManager::recordRollback: on match entity with network id {}", enData.entityId);
					continue;
				}
				en.value().get().forCurrentSerialization(compData.typeId, [this, &compData](sl::Serializable& s) {
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
	auto entities = entityStorage.getEntities();

	for (size_t i = 0; entities.size() > i; ++i) {
		auto it = std::find(typeIds.begin(), typeIds.end(), entities[i].getId());
		if (it == typeIds.end()) {
			OnEntityAbsenceOnStatusPacket.broadcast(entities[i].getId());
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
