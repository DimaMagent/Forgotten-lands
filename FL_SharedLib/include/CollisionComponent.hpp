#pragma once
#include "Component.hpp"
#include "Aabb.hpp"
#include <cstdint>
#include <string>
#include "Cell.hpp"

namespace sl {

	enum class CollisionType: uint8_t {
		None,
		Block,
		Overlapped,
		Ignore
	};

	CollisionType stringToCollisionType(const std::string& str);

	class CollisionComponent : public Component {
	public:
		CollisionComponent(float width, float height, bool isSataticCollisioner, CollisionType collisionType);

		AABB getAABB() const { return aabb; }

		bool isStaticCollisioner() const { return isSataticCollisioner; }

		CollisionType isRelativeCollisionWith(float posX, float posY, AABB otherAABB, float otherPosX, float otherPosY) const;

		void setOccupiedCells(const std::vector<sl::Cell>& cells) { occupiedCells = cells; }
		void addOccupiedCell(const sl::Cell& cell) { occupiedCells.push_back(cell); }
		const std::vector<sl::Cell>& getOccupiedCells() const { return occupiedCells; }
		void occupiedCellsClear() { occupiedCells.clear(); }

		COMPONENT_TYPE(CollisionComponent);
	private:
		AABB aabb;

		bool isSataticCollisioner;

		CollisionType collisionType;

		//stores cells occupied by aabb in CollisionMap
		std::vector<sl::Cell> occupiedCells;

		AABB getRelativeAABB(AABB aabb, float posX, float posY) const;
	};

}