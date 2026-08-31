#pragma once
#include "SystemUpdater.hpp"

class World;
class Serializer;

class ServerSystemUpdater: public sl::SystemUpdater {
public:
	ServerSystemUpdater();

	//temp
	Serializer* getSerializer() const { return serializer.get(); }

protected:
	std::unique_ptr<Serializer> serializer;

	virtual void onUpdate(float deltaTime, const sl::WorldBase& world) override;

	virtual void onEnitiesUpdate(float deltaTime, const sl::WorldBase& world, const sl::Entity& entity) override;

};