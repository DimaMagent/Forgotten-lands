#pragma once
#include <memory>

class DataQueue;

class InputManager {
public:
	InputManager(std::shared_ptr<DataQueue> incQueue);
private:
	std::shared_ptr<DataQueue> incomingQueue;
};