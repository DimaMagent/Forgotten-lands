#pragma once
#include <memory>

class DataQueue;

class OutputManager {
public:
	OutputManager(std::shared_ptr<DataQueue> outQueue);
private:
	std::shared_ptr<DataQueue> outgoingQueue;
};