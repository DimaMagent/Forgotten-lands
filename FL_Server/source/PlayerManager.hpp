#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <unordered_map>
#include <functional>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include "PacketDataTypes.hpp"
#include "Intentions.hpp"

namespace sl {
	class Entity;
	class AttackSystem;
}
class World;

class PlayerManager {
public:
	PlayerManager();
	~PlayerManager();

	void updatePlayerInputState(uint32_t playerToken, sl::Intentions intentions);

	void tick(float dt, const World& world);
private:

	struct PlayerIntention {
		PlayerIntention(sl::Intentions intentions, uint32_t playerToken);
		sl::Intentions intentions;
		uint32_t playerToken;
	};

	std::vector<PlayerIntention> playerIntents;

	std::unique_ptr<sl::AttackSystem> attackSystem;

	const sf::Time updateTime = sf::seconds(1.f / 30.f);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	[[nodiscard]] bool movementUpdate(const sl::Entity& entity, sf::Vector2i movementDirectionIntentions);

	void intentionCheck(const sl::Entity& entity, sl::Intentions intentions, const World& world);
};