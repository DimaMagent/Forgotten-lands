#pragma once
#include <cstdint>
#include <memory>
#include <atomic>
#include "PacketDataTypes.hpp"

class OutputDataManager;
namespace sl {
	class Packet;
}

/*Before using, you must use the setOutputManager method for correct operation.*/
class Packer {
public:
	Packer() = delete;
	static void setOutputManager(std::shared_ptr<OutputDataManager> manager);

	template<typename PacketT, typename... Args>
	static void send( Args&&... args) {
		static_assert(requires { PacketT::type(); }, "PacketT must have static type() method");
		static_assert(requires {typeid(PacketT) == typeid(sl::Packet); }, "PacketT must belong to the Packet class.");
		if (auto manager = staticOutputManager.lock()) {
			PacketT packet;
			uint32_t token = 0;
			packet.fillPacketData(sequenceNumber++, PacketT::type(), token, std::forward<Args>(args)...);
			callOutputManager(packet);
		}
		else {
			std::cerr << "Failed to package input state packet: output manager is no longer available." << "\n";
		}
	}
private:
	static std::atomic<uint16_t> sequenceNumber;
	static std::atomic<uint32_t> currentToken;
	static std::weak_ptr<OutputDataManager> staticOutputManager;

	static void callOutputManager(sl::Packet& packet);
};