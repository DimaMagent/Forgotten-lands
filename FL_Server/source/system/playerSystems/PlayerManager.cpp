#include "pch.hpp"
#include "PlayerManager.hpp"
#include "system/Entity.hpp"
#include "system/Utils.hpp"
#include "network/packets/PacketManager.hpp"
#include "network/packets/PlayerIntentionsPacket.hpp"
#include "system/systems/IntentComponent.hpp"
#include "system/IEntityTokenRegistry.hpp"

PlayerManager::PlayerManager(DataProcessedDelegate& OnDataProcessed, const IEntityTokenRegistry& entityTokenRegistry):
	entityTokenRegistry(entityTokenRegistry)
{
	delegateToken = OnDataProcessed.addFunction([this](sl::net::PacketType type, std::vector<uint8_t>&& data, uint32_t token) {
		if (type == sl::net::PacketType::PT_InputState) {
			sl::net::PlayerIntentionsPacket pkt;
			sl::net::PacketManager::read(data, pkt);
			const auto& intentions = pkt.getData();
			this->updatePlayerInputState(token, intentions.intentions);
		}
		});
}

PlayerManager::~PlayerManager() = default;

void PlayerManager::updatePlayerInputState(uint32_t playerToken, const sl::Intentions& intentions)
{
	auto entity = entityTokenRegistry.getPlayerEntityByToken(playerToken);
	if (!entity.has_value()) { return; }

	sl::IntentComponent* intenComp = entity.value().get().getComponent<sl::IntentComponent>();
	if (!intenComp) { return; }

	intenComp->setIntentions(intentions);
}