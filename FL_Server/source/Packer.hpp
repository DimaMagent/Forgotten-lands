#pragma once
#include <cstdint>
#include <memory>
#include <atomic>
#include "PacketDataTypes.hpp"

class OutputDataManager;
namespace sl::net {
	class Packet;
}

/*Before using, you must use the setOutputManager method for correct operation.*/
class Packer {
public:
	Packer() = delete;
	static void setOutputManager(std::shared_ptr<OutputDataManager> manager);

	template<typename PacketT, typename... Args>
	static void send(Args&&... args) {
		static_assert(requires { PacketT::type(); }, "PacketT must have static type() method");
		static_assert(requires { std::declval<PacketT>().fillPacketData(std::declval<uint16_t>(), std::declval<uint32_t>(), std::declval<Args>()...); },
			"PacketT must have fillPacketData(uint16_t, uint32_t, args...) method.");
		static_assert(std::is_base_of_v<sl::net::Packet, PacketT>, "PacketT must inherit from Packet.");
		if (auto manager = staticOutputManager.lock()) {
			PacketT packet;
			uint32_t token = 0;
			packet.fillPacketData(sequenceNumber++, token, std::forward<Args>(args)...);
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

	static void callOutputManager(sl::net::Packet& packet, uint32_t token);
};