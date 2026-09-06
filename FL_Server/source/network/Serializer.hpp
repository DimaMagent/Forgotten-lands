#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include <span>
#include "LockFreeDelegate.hpp"


namespace sl {
	class Entity;
	struct EntityStorage;
}
class IEntityTokenRegistry;

class Serializer {
public:
	Serializer();
	
	void onUpdate(float updateTime, std::span<const sl::Entity> entities, const IEntityTokenRegistry& entityRegistry);

	std::vector<uint8_t> serializeEntity(const sl::Entity& en) const;
private:
	static int serializationFrequency;
	int serializationCounter = 0;

	void serializeObjects(std::span<const sl::Entity> entities, const IEntityTokenRegistry& entityRegistry) const;


};