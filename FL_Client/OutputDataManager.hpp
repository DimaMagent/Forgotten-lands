#pragma once
#include <memory>

class DataQueue;

class OutputDataManager {
public:
	OutputDataManager(std::shared_ptr<DataQueue> outQueue);
private:
	std::shared_ptr<DataQueue> outgoingQueue;
};