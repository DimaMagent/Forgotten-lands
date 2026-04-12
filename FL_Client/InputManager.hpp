#pragma once
#include <memory>

namespace sf {
	class Event;
}

class InputManager {
	public:
	InputManager(bool& isRunningFlag);
	~InputManager();
	void handleEvent(const sf::Event& event);
private:
	bool& isRunningFlag;
};