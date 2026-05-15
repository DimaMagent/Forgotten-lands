#include "pch.hpp"
#include "Serializer.hpp"
#include "NetUtils.hpp"
#include "Packer.hpp"
#include "StatusPacket.hpp"
#include "PlayerEntityStorage.hpp"
#include "Entity.hpp"
#include "Serializable.hpp"

int Serializer::serializationFrequency = 20;

Serializer::Serializer(sl::LockFreeDelegate<float>& onUpdateDelegate, const PlayerEntityStorage& serializedData):
	entitiesStorage(serializedData)
{
	onUpdateDelegate.addFunction([this](float updateTime) { onUpdate(updateTime); });
	
}

void Serializer::serializeObjects()
{
	std::vector<uint8_t> localBuf;

	for (size_t i = 0; i < entitiesStorage.playerEntities.size(); ++i) {
		entitiesStorage.playerEntities[i]->forEachSerialization([&localBuf](const sl::Serializable& s) {
			uint32_t size = s.getSerializeDataSize();
			sl::net::write_uint32_t(localBuf, size);
			s.serialize(localBuf);
		});
		if (auto it = entitiesStorage.indexToToken.find(i); it != entitiesStorage.indexToToken.end()) {
			Packer::send<sl::net::StatusPacket>(it->second, localBuf);
		}
	}
	localBuf.clear();
	
}

void Serializer::onUpdate(float updateTime) {
	if (serializationFrequency <= ++serializationCounter) {
		serializationCounter = 0;
		serializeObjects();
	}
}