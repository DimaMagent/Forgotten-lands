#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "LockFreeDelegate.hpp"


namespace sl {
	class Entity;
	struct EntityStorage;
}

class Serializer {
public:
	Serializer();
	
	void onUpdate(float updateTime, const sl::EntityStorage& entitiesStorage);
private:
	static int serializationFrequency;
	int serializationCounter = 0;

	void serializeObjects(const sl::EntityStorage& entitiesStorage);


};