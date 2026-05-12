#pragma once
#include "WorldBase.hpp"
#include "LockFreeDelegate.hpp"
#include "PlayerEntityStorage.hpp"
#include <unordered_map>
#include <cstdint>

class Serializer;



class World : public sl::WorldBase {
public:
	sl::LockFreeDelegate<float> OnUpdate;
	World();
	virtual ~World();
	void addPlayerEntity(std::unique_ptr<sl::Entity>&& entity, const uint32_t& sessionToken);
	bool removePlayerEntityUsingToken(const uint32_t& sessionToken);
	bool removePlayerEntityUsingIndex(const size_t& index);
	std::weak_ptr<sl::Entity> getPlayerEntityToToken(uint32_t token) const;
protected:
	virtual void onUpdate(float updateTime) override;

private:
	std::unique_ptr<Serializer> serializer;
	PlayerEntityStorage playerEntityStorage;
};
