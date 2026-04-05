#pragma once
#include <memory>

class IncomingQueue;

class InputManager {
public:
	InputManager(std::shared_ptr<IncomingQueue> incQueue);
private:
	std::shared_ptr<IncomingQueue> incomingQueue;
};