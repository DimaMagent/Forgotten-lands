#pragma once

namespace sl {
	struct AABB {
		float topX;
		float topY;
		float downX;
		float downY;
		AABB(float topX, float topY, float downX, float downY)
			: topX(topX), topY(topY), downX(downX), downY(downY) {}


		bool intersects(const AABB& other) const {
			return !(downX < other.topX || topX > other.downX ||
				downY < other.topY || topY > other.downY);
		}
	};
}