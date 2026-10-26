#include "pch.h"
#include "network/packets/PlayerIntentionsPacket.hpp"
#include "network/NetUtils.hpp"
#include "system/systems/Action.hpp"
#include <type_traits>

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
	net::write<std::underlying_type_t<sl::Action>>(out, intentions.action);
	net::write<bool>(out, intentions.isActionActive);
	return true;
}

void sl::net::PlayerIntentionsData::read(const std::vector<uint8_t>& in, size_t& offset)
{
	if (in.size() >= offset + sl::net::Header::HEADER_SIZE + sizeof(intentions.movementDirectionIntentions.x) + sizeof(intentions.movementDirectionIntentions.y) + sizeof(intentions.action) + sizeof(intentions.isActionActive)){
		header.read(in, offset);
		intentions.movementDirectionIntentions.x = net::read<uint32_t>(in, offset);
		intentions.movementDirectionIntentions.y = net::read<uint32_t>(in, offset);
		intentions.action = static_cast<sl::Action>(net::read<std::underlying_type_t<sl::Action>>(in, offset));
		intentions.isActionActive = net::read<bool>(in, offset);
	}
}

void sl::net::PlayerIntentionsData::fillPacketData(uint16_t sequenceNumber, PacketType type, uint32_t token, sl::Intentions intentions)
{
	this->intentions.movementDirectionIntentions = intentions.movementDirectionIntentions;
	this->intentions.action = intentions.action;
	this->intentions.isActionActive = intentions.isActionActive;
	header.fillHeader(sequenceNumber, type, token, sizeof(intentions.movementDirectionIntentions.x) + sizeof(intentions.movementDirectionIntentions.y) + sizeof(intentions.action) + sizeof(intentions.isActionActive));
}
