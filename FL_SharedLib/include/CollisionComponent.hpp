#pragma once
#include "Component.hpp"
#include "Serializable.hpp"
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

	class CollisionComponent : public Component, public sl::Serializable {
	public:
		CollisionComponent();

		CollisionComponent(float width, float height, bool staticCollisioner, CollisionType collisionType);

		AABB getAABB() const { return aabb; }

		bool isStaticCollisioner() const { return staticCollisioner; }

		CollisionType isRelativeCollisionWith(float posX, float posY, const AABB& otherAABB, float otherPosX, float otherPosY) const;

		void setOccupiedCells(const std::vector<sl::Cell>& cells) { occupiedCells = cells; }

		void addOccupiedCell(const sl::Cell& cell) { occupiedCells.push_back(cell); }

		const std::vector<sl::Cell>& getOccupiedCells() const { return occupiedCells; }

		void occupiedCellsClear() { occupiedCells.clear(); }

		CollisionType getCollisionType() const { return collisionType; }

		virtual void serialize(std::vector<uint8_t>& out) const override;

		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;

		virtual uint32_t getSerializeDataSize() const override;

		virtual uint32_t getDeserializeDataSize() const override;

		COMPONENT_TYPE(CollisionComponent);
	private:
		AABB aabb;

		bool staticCollisioner;

		CollisionType collisionType;

		//stores cells occupied by aabb in CollisionMap
		std::vector<sl::Cell> occupiedCells;

		AABB getRelativeAABB(const AABB& aabb, float posX, float posY) const;

		void writeAABB(std::vector<uint8_t>& out, const AABB& aabb) const;
		AABB readAABB(const std::vector<uint8_t>& in, size_t& offset);

		REGISTER_COMPONENT();
	};

}