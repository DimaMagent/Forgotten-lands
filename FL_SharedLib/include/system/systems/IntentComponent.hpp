#pragma once
#include "system/systems/Component.hpp"
#include "network/packets/Intentions.hpp"
#include <span>
#include <iostream>

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
		const std::span<sl::Action> getActions() { return intentions.actions; }
		const sf::Vector2i& getMovementDirectionIntentions() const { return intentions.movementDirectionIntentions; }
		bool hasAction(sl::Action action) const;
		void addAction(sl::Action action) { 
			intentChanged = true;
			intentions.actions.push_back(action); 
		}
		void setMovementDirection(const sf::Vector2i& newDirection) {
			intentChanged = true;
			intentions.movementDirectionIntentions = newDirection; 
		}
		void clearActions() { 
			intentChanged = false;
			intentions.actions.clear(); 
		}

		bool isIntentChanged() const { return intentChanged; }

		COMPONENT_TYPE(IntentComponent);

	private:
		sl::Intentions intentions;
		bool intentChanged = false;

		REGISTER_COMPONENT();
	};
}