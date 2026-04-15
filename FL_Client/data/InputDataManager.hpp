#pragma once
#include <memory>

class DataQueue;

class InputDataManager {
public:
	InputDataManager(std::shared_ptr<DataQueue> incQueue);
private:
	std::shared_ptr<DataQueue> incomingQueue;
};