#pragma once
#include "system/systems/Component.hpp"
#include "network/Serializable.hpp"
#include "system/systems/collision/Aabb.hpp"
#include <cstdint>
#include <string>
#include "system/Cell.hpp"
#include "system/handlers/LockFreeDelegate.hpp"

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
		sl::LockFreeDelegate<const sl::AABB&> OnCollisionShapedChanged;

		CollisionComponent();

		CollisionComponent(float width, float height, bool staticCollisioner, CollisionType collisionType);

		AABB getAABB() const { return aabb; }

		void setAABB(sl::AABB aabb);

		bool isStaticCollisioner() const { return staticCollisioner; }

		CollisionType isRelativeCollisionWith(float posX, float posY, const AABB& otherAABB, float otherPosX, float otherPosY) const;

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

		AABB getRelativeAABB(const AABB& aabb, float posX, float posY) const;

		void writeAABB(std::vector<uint8_t>& out, const AABB& aabb) const;
		AABB readAABB(const std::vector<uint8_t>& in, size_t& offset);

		REGISTER_COMPONENT();
	};

}