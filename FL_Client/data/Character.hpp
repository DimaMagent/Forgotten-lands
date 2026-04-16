#pragma once
#include <memory>
#include "Entity.hpp"

class MovementComponent;

class Character : public Entity {
public:
	Character(const RenderComponent& rc, const TransformComponent& tc, const MovementComponent& mc);
	~Character();
	void move(float deltaTime);
private:
	std::unique_ptr<MovementComponent> movementComponent;
};