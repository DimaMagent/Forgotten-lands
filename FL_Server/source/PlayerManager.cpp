#include "pch.hpp"
#include "PlayerManager.hpp"
#include "MovementComponent.hpp"
#include "TransformComponent.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include "Utils.hpp"
#include "AttackSystem.hpp"

PlayerManager::PlayerManager()
{
	attackSystem = std::make_unique<sl::AttackSystem>();
}

PlayerManager::~PlayerManager() = default;

void PlayerManager::updatePlayerInputState(uint32_t playerToken, sl::Intentions intentions)
{
	playerIntents.emplace_back(PlayerIntention(intentions, playerToken));
}

bool PlayerManager::movementUpdate(const sl::Entity& entity, sf::Vector2i movementDirectionIntentions)
{
	sl::MovementComponent* movComp = entity.getComponent<sl::MovementComponent>();
	if (!movComp) { return false; }

	movComp->setVelocityDirection(sl::inBounds(movementDirectionIntentions, sf::Vector2i(-1, -1), sf::Vector2i(1, 1)));

	return true;
}

void PlayerManager::intentionCheck(const sl::Entity& entity, sl::Intentions intentions, const World& world)
{
	bool isSuccess = movementUpdate(entity, intentions.movementDirectionIntentions);

	#ifdef DEBUG
		if (!isSuccess) {
			std::shared_ptr<spdlog::logger> system_logger = spdlog::get("system");
			if (system_logger) {
				system_logger->warn("PlayerManager::intentionCheck: movementUpdate call is not successfully");
			}
		}
	#endif // DEBUG

	for (auto& action : intentions.actions) {
		if (action == sl::net::Action::None) { continue; }

		if (action == sl::net::Action::Attack)
		{
			if (!attackSystem) { continue; }

			isSuccess = attackSystem->tryMeleeAttack(entity, world);

			#ifdef DEBUG
				if (!isSuccess) {
					std::shared_ptr<spdlog::logger> system_logger = spdlog::get("system");
					if (system_logger) {
						system_logger->warn("PlayerManager::intentionCheck: tryMeleeAttack call is not successfully");
					}
				}
			#endif // DEBUG
		}
	}
}

void PlayerManager::tick(float dt, const World& world) {
	timeSinceLastUpdate += std::min(sf::seconds(dt), sf::seconds(0.1f));
	while (timeSinceLastUpdate >= updateTime) {
		timeSinceLastUpdate -= updateTime;

		for (auto& playerIntention : playerIntents) {

			auto entity = world.getPlayerEntityByToken(playerIntention.playerToken);
			if (!entity.has_value()) { return; }

			intentionCheck(entity.value(), playerIntention.intentions, world);
		}
		playerIntents.clear();
	}
}

PlayerManager::PlayerIntention::PlayerIntention(sl::Intentions intentions, uint32_t playerToken)
{
	this->intentions = intentions;
	this->playerToken = playerToken;
}
