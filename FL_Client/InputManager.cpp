#include "InputManager.hpp"
#include "DataQueue.hpp"

InputManager::InputManager(std::shared_ptr<DataQueue> incQueue):
	incomingQueue(std::move(incQueue))
{
}
