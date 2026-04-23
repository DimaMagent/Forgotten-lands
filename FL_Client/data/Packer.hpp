#pragma once
#include <cstdint>
#include <memory>
#include <atomic>
#include "PacketDataTypes.hpp"

class OutputDataManager;

/*Before using, you must use the setOutputManager method for correct operation.*/
class Packer {
public:
	Packer() = delete;
	static void packageInputStatePacket(sl::InputState inputState);
	static void setOutputManager(std::shared_ptr<OutputDataManager> manager);
private:
	static std::atomic<uint16_t> secuenceNumber;
	static std::weak_ptr<OutputDataManager> staticOutputManager;
};