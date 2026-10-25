#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <unordered_map>
#include <functional>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include "network/packets/Intentions.hpp"
#include "network/DataProcessorTypes.hpp"

namespace sl {
	class Entity;
}
class IEntityTokenRegistry;

class PlayerManager {
public:
	PlayerManager(DataProcessedDelegate& OnDataProcessed, const IEntityTokenRegistry& entityTokenRegistry);
	~PlayerManager();

	void updatePlayerInputState(uint32_t playerToken, const sl::Intentions& intentions);

private:

	const IEntityTokenRegistry& entityTokenRegistry;

	std::shared_ptr<sl::DelegateToken> delegateToken;
};