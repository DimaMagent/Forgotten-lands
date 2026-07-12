#pragma once
#include <vector>
#include <cstdint>
#include <memory>
#include "LockFreeDelegate.hpp"

struct PlayerEntityStorage;
namespace sl {
	class Entity;
}

class Serializer {
public:
	Serializer();
	
	void onUpdate(float updateTime, const PlayerEntityStorage& entitiesStorage);
private:
	static int serializationFrequency;
	int serializationCounter = 0;

	void serializeObjects(const PlayerEntityStorage& entitiesStorage);


};