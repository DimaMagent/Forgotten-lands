#include "InputDataManager.hpp"
#include "DataQueue.hpp"

InputDataManager::InputDataManager(std::shared_ptr<DataQueue> incQueue):
	incomingQueue(std::move(incQueue))
{
}
