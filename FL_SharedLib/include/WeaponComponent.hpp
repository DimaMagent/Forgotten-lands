#pragma once
#include "Component.hpp"
#include <SFML/System/Vector2.hpp>
#include "Serializable.hpp"

namespace sl {

	class WeaponComponent: public Component, public Serializable {
	public:
		WeaponComponent();
		WeaponComponent(float attackDistance, float attackDegrees, float attackDamage);
		
		float getAttackDistance() const { return attackDistance; }
		float getAttackDegrees() const { return attackDegrees; }
		float getAttackDamage() const { return attackDamage; }

		virtual void serialize(std::vector<uint8_t>& out) const override;
		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;
		virtual uint32_t getSerializeDataSize() const override;
		virtual uint32_t getDeserializeDataSize() const override;

		COMPONENT_TYPE(WeaponComponent);
	private:
		float attackDistance;
		float attackDegrees;
		float attackDamage;
	};

}