#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "LockFreeDelegate.hpp"


namespace sl {
	class Entity;
	struct EntityStorage;
}
class World;

class Serializer {
public:
	Serializer(const World& world);
	
	void onUpdate(float updateTime);

	std::vector<uint8_t> serializeEntity(const sl::Entity& en) const;
private:
	static int serializationFrequency;
	int serializationCounter = 0;

	const World& world;

	void serializeObjects() const;


};