#include "pch.hpp"
#include "OutputDataManager.hpp"
#include "Packer.hpp"
#include "Header.hpp"
#include "InputStatePacket.hpp"

std::atomic<uint16_t> Packer::sequenceNumber = 0;
std::weak_ptr<OutputDataManager> Packer::staticOutputManager;

void Packer::setOutputManager(std::shared_ptr<OutputDataManager> manager)
{
	staticOutputManager = manager;
}

void Packer::packageInputStatePacket(sl::InputState inputState, bool pressingFlag)
{
	if (auto manager = staticOutputManager.lock()) {
		sl::InputStatePacket packet;
		uint32_t token = 0;
		packet.fillPacketData(sequenceNumber++, sl::PacketType::PT_InputState, token, static_cast<uint8_t>(inputState), pressingFlag);
		manager->writePacket(packet);
	}
	else {
		std::cerr << "Failed to package input state packet: output manager is no longer available." << "\n";
	}
}
