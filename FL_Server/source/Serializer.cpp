#include "pch.hpp"
#include "Serializer.hpp"
#include "NetUtils.hpp"
#include "Packer.hpp"
#include "StatusPacket.hpp"

void Serializer::serializeObjects(const std::vector<sl::Serializable*>& serializableObject)
{
	if (isBufAlreadyFull) {
		std::cerr << "Serializer::serializeObjects: local buffer already full\n";
	}
	for (auto& obj : serializableObject) {
		uint32_t size = obj->getSerializeDataSize();
		sl::net::write_uint32_t(localBuf, size);
		obj->serialize(localBuf);
	}
	isBufAlreadyFull = true;
	
}

void Serializer::clearLocalBuf()
{
	isBufAlreadyFull = false;
	localBuf.clear();
}

void Serializer::sendSerializeData(uint32_t token) const
{
	Packer::send<sl::net::StatusPacket>(token, localBuf);
}
