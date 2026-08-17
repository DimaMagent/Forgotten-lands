#pragma once
#include "LockFreeDelegate.hpp"
#include <vector>
#include "SFML/System/Vector2.hpp"

namespace sl {
	class Entity;
	class CollisionCellMap;
	class WorldBase;
	struct EntityStorage;
	enum class CollisionType : uint8_t;

	class CollisionSystem {
	public:
		CollisionSystem(const sl::CollisionCellMap& collisionCellMap, const WorldBase& world);
		void onUpdate(float updateTime);
		bool isBlockedOnPosition(sl::Entity& entity, const sf::Vector2f& testPos);

		sl::LockFreeDelegate<sl::Entity&, sl::Entity&, sl::CollisionType> onCollisionDetected;
	private:
		float updateRate = 1.0f / 10.0f;
		float currentTime = 0.f;

		const int searchDepth = 1;
		const int onPositionSearchDepth = 1;
		const float delta = 1.f;

		std::vector<uint32_t> reusableEntityIdsBuffer;

		const sl::CollisionCellMap& collisionCellMap;
		const WorldBase& world;
	};
}