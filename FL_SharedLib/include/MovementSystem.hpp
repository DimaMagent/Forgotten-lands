#pragma once

namespace sl {
	class Entity;
	class CollisionSystem;

	class MovementSystem {
	public:
		MovementSystem(CollisionSystem& collisionSystem);

		void onUpdate(sl::Entity& entity, float updateTime) const;
	private:
		CollisionSystem& collisionSystem; //temp
		void movingWithCollisionCheck(sl::Entity& entity, float updateTime) const;
	};
}