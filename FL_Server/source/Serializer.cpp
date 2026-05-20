#include "pch.hpp"
#include "Serializer.hpp"
#include "NetUtils.hpp"
#include "Packer.hpp"
#include "StatusPacket.hpp"
#include "PlayerEntityStorage.hpp"
#include "Entity.hpp"
#include "Serializable.hpp"

int Serializer::serializationFrequency = 5;

Serializer::Serializer(sl::LockFreeDelegate<float>& onUpdateDelegate, const PlayerEntityStorage& serializedData):
	entitiesStorage(serializedData)
{
	onUpdateDelegate.addFunction([this](float updateTime) { onUpdate(updateTime); });
	
}

void Serializer::serializeObjects()
{
	std::vector<uint8_t> localBuf;

	for (size_t i = 0; i < entitiesStorage.playerEntities.size(); ++i) {
		uint32_t entityId = entitiesStorage.playerEntities[i]->getId();
		sl::net::write_uint32_t(localBuf, entityId);

		std::vector<uint8_t> entityLocalBuf;
		uint32_t entityDataSize = 0;

		entitiesStorage.playerEntities[i]->forEachSerialization([&entityLocalBuf, &entityDataSize](const sl::Serializable& s) {
			entityDataSize += s.getSerializeDataSize();
			s.serialize(entityLocalBuf);
		});

		sl::net::write_uint32_t(localBuf, entityDataSize);
		localBuf.insert(localBuf.end(), entityLocalBuf.begin(), entityLocalBuf.end());
	}

	for (size_t i = 0; i < entitiesStorage.playerEntities.size(); ++i) {
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