#include "pch.hpp"
#include "IncomingDataManager.hpp"
#include "DataQueue.hpp"
#include "PacketManager.hpp"

IncomingDataManager::IncomingDataManager(std::shared_ptr<sl::DataQueue> incQueue):
	incomingQueue(std::move(incQueue))
{
}
