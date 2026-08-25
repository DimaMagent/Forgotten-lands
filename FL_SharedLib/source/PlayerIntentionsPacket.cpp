#include "pch.h"
#include "PlayerIntentionsPacket.hpp"
#include "NetUtils.hpp"

bool sl::net::PlayerIntentionsPacket::write(std::vector<uint8_t>& out) const
{
	return data.write(out);
}

void sl::net::PlayerIntentionsPacket::read(const std::vector<uint8_t>& in, size_t& offset)
{
	data.read(in, offset);
}


void sl::net::PlayerIntentionsPacket::fillPacketData(uint16_t sequenceNumber, uint32_t token, sl::Intentions intentions)
{
	data.fillPacketData(sequenceNumber, type(), token, intentions);
}

bool sl::net::PlayerIntentionsData::write(std::vector<uint8_t>& out) const
{
	header.write(out);
	net::write<uint32_t>(out, intentions.movementDirectionIntentions.x);
	net::write<uint32_t>(out, intentions.movementDirectionIntentions.y);
	net::write<uint8_t>(out, static_cast<uint8_t>(intentions.actions.size()));
	for (auto action: intentions.actions) {
		net::write<uint8_t>(out, static_cast<uint8_t>(action));
	}
	return true;
}

void sl::net::PlayerIntentionsData::read(const std::vector<uint8_t>& in, size_t& offset)
{
	if (in.size() >= offset + sl::net::Header::HEADER_SIZE + sizeof(intentions.movementDirectionIntentions.x) + sizeof(intentions.movementDirectionIntentions.y) + sizeof(actionsSize)){
		header.read(in, offset);
		intentions.movementDirectionIntentions.x = net::read<uint32_t>(in, offset);
		intentions.movementDirectionIntentions.y = net::read<uint32_t>(in, offset);
		actionsSize = net::read<uint8_t>(in, offset);
		for (uint8_t i = 0; i < actionsSize; ++i) {
			if (offset + sizeof(uint8_t) <= in.size()) {
				intentions.actions.push_back(static_cast<sl::net::Action>(net::read<std::underlying_type_t<sl::net::Action>>(in, offset)));
			}
			else {
				break;
			}

		}
	}
}

void sl::net::PlayerIntentionsData::fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, sl::Intentions intentions)
{
	this->intentions.movementDirectionIntentions = intentions.movementDirectionIntentions;
	this->intentions.actions = intentions.actions;
	header.fillHeader(sequenceNumber, type, token, intentions.actions.size() * sizeof(decltype(intentions.actions)::value_type) + sizeof(intentions.movementDirectionIntentions.x) + sizeof(intentions.movementDirectionIntentions.y) + sizeof(actionsSize));
}
