#pragma once
#include <memory>

class OutgoingQueue;

class OutputManager {
public:
	OutputManager(std::shared_ptr<OutgoingQueue> outQueue);
private:
	std::shared_ptr<OutgoingQueue> outgoingQueue;
};