#pragma once

namespace sl {
	class Entity;
	class ColisionCellMap;

	class CollisionSystem {
	public:
		CollisionSystem(const sl::ColisionCellMap& collisionCellMap);
		void onUpdate(sl::Entity& entity, float updateTime);
	private:
		float updateRate = 1 / 10;
		float currentTime = 0.f;

		const sl::ColisionCellMap& collisionCellMap;
	};
}