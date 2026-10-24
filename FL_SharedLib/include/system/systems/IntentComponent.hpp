#pragma once
#include "system/systems/Component.hpp"
#include "network/packets/Intentions.hpp"
#include <span>

namespace sl {
	class IntentComponent : public sl::Component {
	public:
		IntentComponent() = default;
		~IntentComponent() override = default;
		void setIntentions(const sl::Intentions& intentions) { this->intentions = intentions; }
		const sl::Intentions& getIntentions() const { return intentions; }
		const std::span<sl::net::Action> getActions() { return intentions.actions; }
		COMPONENT_TYPE(IntentComponent);

	private:
		sl::Intentions intentions;

		REGISTER_COMPONENT();
	};
}