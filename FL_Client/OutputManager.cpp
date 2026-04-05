#include "OutputManager.hpp"
#include "OutgoingQueue.hpp"

OutputManager::OutputManager(std::shared_ptr<OutgoingQueue> outQueue):
	outgoingQueue(std::move(outQueue))
{
}
