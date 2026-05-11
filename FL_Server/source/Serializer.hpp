#pragma once
#include <vector>
#include <cstdint>
#include "Serializable.hpp"


// This class needs to be rewritten. Because he is not independent
class Serializer {
public:
	Serializer() = default;
	void serializeObjects(const std::vector<sl::Serializable*>& serializableObject);
	void sendSerializeData(uint32_t token)const;
	void clearLocalBuf();
private:
	std::vector<uint8_t> localBuf;
	bool isBufAlreadyFull = false;
};