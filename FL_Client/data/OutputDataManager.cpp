#include "OutputDataManager.hpp"
#include "DataQueue.hpp"

OutputDataManager::OutputDataManager(std::shared_ptr<DataQueue> outQueue):
	outgoingQueue(std::move(outQueue))
{
}
