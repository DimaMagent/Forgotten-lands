#pragma once
#include "Component.hpp"
#include "Serializable.hpp"
#include "FlagCounterConteiner.hpp"
#include <unordered_map>

namespace sl {

	enum class StunState : uint8_t {
		None = 0,
		Immobilized = 1 << 0,
		Disarmed = 1 << 1
	};

	inline StunState operator|(StunState a, StunState b) {
		using T = std::underlying_type_t<StunState>;
		return static_cast<StunState>(static_cast<T>(a) | static_cast<T>(b));
	}

	inline StunState operator&(StunState a, StunState b) {
		using T = std::underlying_type_t<StunState>;
		return static_cast<StunState>(static_cast<T>(a) & static_cast<T>(b));
	}

	inline StunState operator~(StunState a) {
		using T = std::underlying_type_t<StunState>;
		return static_cast<StunState>(~static_cast<T>(a));
	}

	inline StunState& operator|=(StunState& a, StunState b) {
		return a = a | b;
	}

	inline StunState& operator&=(StunState& a, StunState b) {
		return a = a & b;
	}

	class StunComponent : public sl::Component, public sl::Serializable {
	public:
		StunComponent();

		// Use if the entity has applied the stun effect to themselves
		void addWithinStun(StunState state);

		// use to add a stun effect from external entities
		void addExternalStun(StunState state, uint32_t stunningEnityId);

		// Use to remove the entity's internal stuns.
		void removeWithinStun(StunState state);

		// use to remove a stun effect from external entities
		void removeExternalStun(StunState state, uint32_t stunningEnityId);

		void removeAllStunsAppliedByEntityId(uint32_t stunningEnityId);

		bool hasStun(StunState state) const;

		virtual void serialize(std::vector<uint8_t>& out) const override;
		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;
		virtual uint32_t getSerializeDataSize() const override;
		virtual uint32_t getDeserializeDataSize() const override;

		COMPONENT_TYPE(StunComponent);
	private:
		StunState currentStuns;

		//stores the stuns effects received from within
		FlagCounterContainer<StunState, 2> flagCounterContainer;

		//stores id of the stuns entity and stunning they inflicted
		std::unordered_map<uint32_t, StunState> activeEffects;


		void calculateCurrentStuns();

	};
}