#pragma once
#include <cstdint>
#include <vector>
#include "Packet.hpp"
#include <SFML/System/Vector2.hpp>
#include "PacketDataTypes.hpp"
#include "Intentions.hpp"

namespace sl::net {

	struct PlayerIntentionsData : public Data {
		sl::Intentions intentions;
		// size of actions cannot exceed uint8_t
		uint8_t actionsSize;

		virtual bool write(std::vector<uint8_t>& out) const override;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
		virtual void fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, sl::Intentions intentions);
	};

	class PlayerIntentionsPacket: public Packet {
		public:
			virtual bool write(std::vector<uint8_t>& out) const override;
			virtual void read(const std::vector<uint8_t>& in, size_t& offset) override;
			static constexpr PacketType type() { return PacketType::PT_InputState; }
			virtual void fillPacketData(uint16_t sequenceNumber, uint32_t token, sl::Intentions intentions);
			const Header& getHeader() const override { return data.header; }
			const PlayerIntentionsData& getData() const override { return data; }
		private:
			PlayerIntentionsData data;
	};

}
