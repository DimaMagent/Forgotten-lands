#include "pch.hpp"
#include "PlayerStateManager.hpp"
#include "StatusPacket.hpp"
#include "Entity.hpp"
#include "TransformComponent.hpp"

PlayerStateManager::PlayerStateManager(sl::Delegate<const std::weak_ptr<sl::Entity>>& onSetPlayerEntityDelegate): playerEntity(playerEntity)
{
	onSetPlayerEntityDelegate.addFunction([this](const std::weak_ptr<sl::Entity> playerEntity) {this->playerEntity = playerEntity; });
}

void PlayerStateManager::recordRollback(const sl::net::StatusData& data)
{
	if (data.header.getData().sequenceNumber <= lastAppliedSequence) { return; }
	++lastAppliedSequence;
	size_t offset = 0;
	uint32_t size = sl::net::read_uint32_t(data.data, offset);
	float x = (float)sl::net::read_uint32_t(data.data, offset);
	float y = (float)sl::net::read_uint32_t(data.data, offset);

	auto player = playerEntity.lock();
	if (!player) { return; }
	
	sl::TransformComponent* trComp = player->getComponent<sl::TransformComponent>();
	trComp->setPosition(x, y);

	std::cout << "Received server data: " << x << " ; " << y << "\n";
	
}
