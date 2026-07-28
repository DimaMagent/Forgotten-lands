#include "pch.hpp"
#include "Serializer.hpp"
#include "NetUtils.hpp"
#include "Packer.hpp"
#include "StatusPacket.hpp"
#include "EntityStorage.hpp"
#include "Entity.hpp"
#include "Serializable.hpp"

int Serializer::serializationFrequency = 2;

Serializer::Serializer()
{	
}

void Serializer::serializeObjects( const sl::EntityStorage& entitiesStorage)
{
	std::vector<uint8_t> localBuf;

	for (size_t i = 0; i < entitiesStorage.getEntities().size(); ++i) {

		std::vector<uint8_t> entityLocalBuf = serializeEntity(*entitiesStorage.getEntities()[i]);

		localBuf.insert(localBuf.end(), entityLocalBuf.begin(), entityLocalBuf.end());
	}

	for (size_t i = 0; i < entitiesStorage.getEntities().size(); ++i) {
		if (uint32_t token = entitiesStorage.getIdToIndex(i); token != 0) {
			Packer::send<sl::net::StatusPacket>(token, localBuf);
		}
	}

	localBuf.clear();
	
}

void Serializer::onUpdate(float updateTime, const sl::EntityStorage& entitiesStorage) {
	if (serializationFrequency <= ++serializationCounter) {
		serializationCounter = 0;
		serializeObjects(entitiesStorage);
	}
}

std::vector<uint8_t> Serializer::serializeEntity(const sl::Entity& en) const
{
	std::vector<uint8_t> localBuf;

	uint32_t entityId = en.getId();
	sl::net::write_uint32_t(localBuf, entityId);

	uint32_t entityType = static_cast<uint32_t>(en.getEntityType());
	sl::net::write_uint32_t(localBuf, entityType);

	std::vector<uint8_t> entityLocalBuf;
	uint32_t entityDataSize = 0;

	en.forEachSerialization([&entityLocalBuf, &entityDataSize](const sl::Serializable& s) {
		entityDataSize += s.getSerializeDataSize();
		s.serialize(entityLocalBuf);
		});

	sl::net::write_uint32_t(localBuf, entityDataSize);

	localBuf.insert(localBuf.end(), entityLocalBuf.begin(), entityLocalBuf.end());

	return localBuf;
}
