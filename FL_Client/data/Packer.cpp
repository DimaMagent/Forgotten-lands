#include "pch.hpp"
#include "OutputDataManager.hpp"
#include "Packer.hpp"
#include "Header.hpp"
#include "InputStatePacket.hpp"

std::atomic<uint16_t> Packer::secuenceNumber = 0;
std::weak_ptr<OutputDataManager> Packer::staticOutputManager;

void Packer::setOutputManager(std::shared_ptr<OutputDataManager> manager)
{
	staticOutputManager = manager;
}

void Packer::packageInputStatePacket(sl::InputState inputState)
{
	if (auto manager = staticOutputManager.lock()) {
		sl::InputStatePacket packet;
		packet.fillPacketData(secuenceNumber++, sl::PacketType::PT_InputState, static_cast<uint8_t>(inputState));
		manager->writePacket(packet);
	}
	else {
		std::cerr << "Failed to package input state packet: output manager is no longer available." << "\n";
	}
}
