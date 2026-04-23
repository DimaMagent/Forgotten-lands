#pragma once
#include <memory>
#include "LockFreeDelegate.hpp"


namespace sf {
	class Event;
}

class InputManager {
public:
	sl::LockFreeDelegate<const sf::Event&> onEvent;
	InputManager(bool& isRunningFlag);
	~InputManager();
	void handleEvent(const sf::Event& event);
private:
	bool& isRunningFlag;

	void mousePosChecker(const sf::Event& event);
};