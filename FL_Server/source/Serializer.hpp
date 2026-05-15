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
	Serializer(sl::LockFreeDelegate<float>& onUpdateDelegate, const PlayerEntityStorage& serializedData);
private:
	const PlayerEntityStorage& entitiesStorage;
	static int serializationFrequency;
	int serializationCounter = 0;

	void serializeObjects();
	void onUpdate(float updateTime);

};