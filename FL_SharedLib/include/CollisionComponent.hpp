#pragma once
#include "Component.hpp"
#include "Aabb.hpp"

namespace sl {

	class CollisionComponent : public Component {
	public:
		CollisionComponent(float width, float height, bool isSataticCollisioner);

		AABB getAABB() const { return aabb; }
		bool isStaticCollisioner() const { return isSataticCollisioner; }

		COMPONENT_TYPE(CollisionComponent);
	private:
		AABB aabb;
		bool isSataticCollisioner;
	};

}