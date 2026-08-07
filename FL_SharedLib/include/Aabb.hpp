#pragma once

namespace sl {
	struct AABB {
		float topX;
		float topY;
		float downX;
		float downY;
		AABB(float topX, float topY, float downX, float downY)
			: topX(topX), topY(topY), downX(downX), downY(downY) {}

		AABB() : topX(-1.f), topY(-1.f), downX(-1.f), downY(-1.f) {}


		bool intersects(const AABB& other) const {
			return !(downX < other.topX || topX > other.downX ||
				downY < other.topY || topY > other.downY);
		}

		bool exists() const {
			return topX >= 0 && topY >= 0 && downX >= 0 && downY >= 0;
		}
	};
}