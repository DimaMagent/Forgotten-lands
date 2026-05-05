#include "pch.hpp"
#include "Packer.hpp"
#include "Header.hpp"
#include "Packet.hpp"
#include "OutputDataManager.hpp"

std::atomic<uint16_t> Packer::sequenceNumber = 0;
std::weak_ptr<OutputDataManager> Packer::staticOutputManager;

void Packer::setOutputManager(std::shared_ptr<OutputDataManager> manager)
{
	staticOutputManager = manager;
}

void Packer::callOutputManager(sl::net::Packet& packet, uint32_t token)
{
	if (auto manager = staticOutputManager.lock()) {
		manager->writePacket(packet, token);
	}
}