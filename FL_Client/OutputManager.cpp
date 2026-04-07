#include "OutputManager.hpp"
#include "DataQueue.hpp"

OutputManager::OutputManager(std::shared_ptr<DataQueue> outQueue):
	outgoingQueue(std::move(outQueue))
{
}
