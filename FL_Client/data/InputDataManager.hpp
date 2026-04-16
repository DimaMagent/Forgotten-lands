#pragma once
#include <memory>

namespace sl {
	class DataQueue;
}

class InputDataManager {
public:
	InputDataManager(std::shared_ptr<sl::DataQueue> incQueue);
private:
	std::shared_ptr<sl::DataQueue> incomingQueue;
};