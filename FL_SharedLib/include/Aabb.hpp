#pragma once

namespace sl {
	struct AABB {
		float minX;
		float minY;
		float maxX;
		float maxY;
		AABB(float minX, float minY, float maxX, float maxY)
			: minX(minX), minY(minY), maxX(maxX), maxY(maxY) {}

		AABB() : minX(-1.f), minY(-1.f), maxX(-1.f), maxY(-1.f) {}


		bool intersects(const AABB& other) const {
			return !(maxX <= other.minX || minX >= other.maxX ||
				maxY <= other.minY || minY >= other.maxY);
		}

		bool exists() const {
			return minX >= 0 && minY >= 0 && maxX >= 0 && maxY >= 0;
		}
	};
}