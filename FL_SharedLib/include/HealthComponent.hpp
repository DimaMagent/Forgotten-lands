#pragma once
#include "Component.hpp"
#include "Serializable.hpp"

namespace sl {

	class HealthComponent : public Component, public Serializable {
	public:
		HealthComponent();

		HealthComponent(float maxHealth);

		float getMaxHealth() const { return maxHealth; }

		float getCurrentHealth() const { return currentHealth; }

		void takeDamage(float damage);

		virtual void serialize(std::vector<uint8_t>& out) const override;
		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;
		virtual uint32_t getSerializeDataSize() const override;
		virtual uint32_t getDeserializeDataSize() const override;

		COMPONENT_TYPE(HealthComponent);
	private:
		float maxHealth;
		float currentHealth;

		REGISTER_COMPONENT();
	};

}