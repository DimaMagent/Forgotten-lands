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
		void setIntentions(const sl::Intentions& intentions) {  this->intentions = intentions; }
		const sl::Intentions& getIntentions() const { return intentions; }
		const std::span<sl::Action> getActions() { std::cout << "show action\n"; return intentions.actions; }
		const sf::Vector2i& getMovementDirectionIntentions() const { return intentions.movementDirectionIntentions; }
		bool hasAction(sl::Action action) const;
		void addAction(sl::Action action) { intentions.actions.push_back(action); }
		void setMovementDirection(const sf::Vector2i& newDirection) { intentions.movementDirectionIntentions = newDirection; }
		void clearActions() { intentions.actions.clear(); }

		COMPONENT_TYPE(IntentComponent);

	private:
		sl::Intentions intentions;

		REGISTER_COMPONENT();
	};
}