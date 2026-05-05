#include "pch.hpp"
#include "IncomingDataManager.hpp"
#include "DataQueue.hpp"
#include "PacketManager.hpp"

IncomingDataManager::IncomingDataManager(std::weak_ptr<sl::net::DataQueue> incQueue):
	incomingQueue(incQueue)
{
}
