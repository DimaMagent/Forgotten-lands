#include "InputManager.hpp"
#include "IncomingQueue.hpp"

InputManager::InputManager(std::shared_ptr<IncomingQueue> incQueue):
	incomingQueue(std::move(incQueue))
{
}
