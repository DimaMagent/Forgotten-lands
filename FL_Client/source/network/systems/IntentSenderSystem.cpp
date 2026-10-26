#include "pch.hpp"
#include "IntentSenderSystem.hpp"
#include "system/IPlayerEntityStorage.hpp"
#include "system/Entity.hpp"
#include "system/systems/IntentComponent.hpp"
#include "network/Packer.hpp"
#include "network/packets/PlayerIntentionsPacket.hpp"
#include "system/systems/ClientSystemCreatorContext.hpp"
#include "network/packets/Intentions.hpp"

IntentSenderSystem::IntentSenderSystem(const IPlayerEntityStorage& playerEntityStorage):
	playerEntityStorage(playerEntityStorage){}

void IntentSenderSystem::update(float updateTime, const sl::Entity& entity)
{
	if (entity.getId() != playerEntityStorage.getPlayerEntityId()) { return; }
	
	sl::IntentComponent* intentComp = entity.getComponent<sl::IntentComponent>();

	if (!intentComp) { return; }

	if (!intentComp->isIntentChanged()) { return; }

	Packer::send<sl::net::PlayerIntentionsPacket>(intentComp->getIntentions());
}

sl::systemPtr IntentSenderSystem::initialize(sl::SystemCreatorContext& context) {
	ClientSystemCreatorContext& clientContext = static_cast<ClientSystemCreatorContext&>(context);
	return std::make_unique<IntentSenderSystem>(clientContext.playerEntityStorage);
}

sl::SystemProcessingPhase IntentSenderSystem::getProcessingPhase() {
	return sl::SystemProcessingPhase::Input;
}