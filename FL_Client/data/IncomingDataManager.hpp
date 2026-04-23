#pragma once
#include <memory>

namespace sl {
	class DataQueue;
}

class IncomingDataManager {
public:
	IncomingDataManager(std::shared_ptr<sl::DataQueue> incQueue);
private:
	std::weak_ptr<sl::DataQueue> incomingQueue;
};