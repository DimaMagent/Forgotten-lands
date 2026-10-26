#pragma once
#include "system/systems/Component.hpp"
#include "network/packets/Intentions.hpp"
#include <span>
#include <iostream>
#include <optional>

namespace sl {
	class IntentComponent : public sl::Component {
	public:
		IntentComponent() = default;
		~IntentComponent() override = default;
		void setIntentions(const sl::Intentions& intentions) {
			intentChanged = true;
			this->intentions = intentions; 
		}
		const sl::Intentions& getIntentions() const { return intentions; }
		const sf::Vector2i& getMovementDirectionIntentions() const { return intentions.movementDirectionIntentions; }
		bool hasAction(sl::Action action) const;
		void setAction(sl::Action action, bool isActionActive) { 
			intentChanged = intentions.isActionActive != isActionActive || action != intentions.action;
			intentions.action = action;
			intentions.isActionActive = isActionActive;
		}
		void setMovementDirection(const sf::Vector2i& newDirection) {
			intentChanged = true;
			intentions.movementDirectionIntentions = newDirection; 
		}
		void clearActions() {
			intentChanged = false;
			if (!intentions.isActionActive) {
				intentions.action = sl::Action::None;
			}
		}

		bool isIntentChanged() const { return intentChanged; }

		COMPONENT_TYPE(IntentComponent);

	private:
		sl::Intentions intentions;
		bool intentChanged = false;

		REGISTER_COMPONENT();
	};
}