#pragma once
#include "WorldBase.hpp"
#include <unordered_map>
#include <cstdint>

namespace sl {
	class Entity;
}

class World : public sl::WorldBase {
public:
	World();
	~World();
private:
	std::unordered_map<uint32_t, std::shared_ptr<sl::Entity>> playersEntity;
};
