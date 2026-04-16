#pragma once
#include <memory>
namespace sl {
	class DataQueue;
}

class OutputDataManager {
public:
	OutputDataManager(std::shared_ptr<sl::DataQueue> outQueue);
private:
	std::shared_ptr<sl::DataQueue> outgoingQueue;
};