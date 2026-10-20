#pragma once
#include "system/systems/SystemUpdater.hpp"

class World;
class Serializer;

class ServerSystemUpdater: public sl::SystemUpdater {
public:
	ServerSystemUpdater(const sl::SystemCreatorContext& context);

protected:

	virtual void onUpdate(float deltaTime) override;

	virtual void onEnitiesUpdate(float deltaTime, const sl::Entity& entity) override;

};