#include "pch.hpp"
#include "Serializer.hpp"
#include "NetUtils.hpp"
#include "Packer.hpp"
#include "StatusPacket.hpp"
#include "EntityStorage.hpp"
#include "Entity.hpp"
#include "IEntityTokenRegistry.hpp"
#include "Serializable.hpp"

int Serializer::serializationFrequency = 2;

Serializer::Serializer(){}

void Serializer::serializeObjects(std::span<const sl::Entity> entities, const IEntityTokenRegistry& entityRegistry) const
{
	std::vector<uint8_t> localBuf;

	for (size_t i = 0; i < entities.size(); ++i) {

		std::vector<uint8_t> entityLocalBuf = serializeEntity(entities[i]);

		localBuf.insert(localBuf.end(), entityLocalBuf.begin(), entityLocalBuf.end());
	}

	for (size_t i = 0; i < entities.size(); ++i) {
		if (auto tokenOpt = entityRegistry.getTokenById(entities[i].getId()); tokenOpt.has_value()) {
			Packer::send<sl::net::StatusPacket>(tokenOpt.value(), localBuf);
		}
	}

	localBuf.clear();
	
}

void Serializer::onUpdate(float updateTime, std::span<const sl::Entity> entities, const IEntityTokenRegistry& entityRegistry) {
	if (serializationFrequency <= ++serializationCounter) {
		serializationCounter = 0;
		serializeObjects(entities, entityRegistry);
	}
}

std::vector<uint8_t> Serializer::serializeEntity(const sl::Entity& en) const
{
	std::vector<uint8_t> localBuf;

	uint32_t entityId = en.getId().ID;
	sl::net::write<uint32_t>(localBuf, entityId);

	uint32_t entityType = static_cast<uint32_t>(en.getEntityType());
	sl::net::write<uint32_t>(localBuf, entityType);

	std::vector<uint8_t> entityLocalBuf;
	uint32_t entityDataSize = 0;

	en.forEachSerialization([&entityLocalBuf, &entityDataSize](const sl::Serializable& s) {
		entityDataSize += s.getSerializeDataSize();
		s.serialize(entityLocalBuf);
		});

	sl::net::write<uint32_t>(localBuf, entityDataSize);

	localBuf.insert(localBuf.end(), entityLocalBuf.begin(), entityLocalBuf.end());

	return localBuf;
}
