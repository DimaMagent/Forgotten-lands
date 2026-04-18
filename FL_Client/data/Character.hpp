#pragma once
#include <memory>
#include "Entity.hpp"
#include <SFML/System/Vector2.hpp>

class MovementComponent;

class Character : public Entity {
public:
	Character() = default;
	Character(const RenderComponent& rc, const TransformComponent& tc, const MovementComponent& mc);
	Character(const Character& other);
	Character(Character&& other) noexcept;
	~Character();
	void move(float deltaTime);
	void addVelocityVector(sf::Vector2i direction, float speed);
	void addDirection(sf::Vector2i direction);
private:
	std::unique_ptr<MovementComponent> movementComponent;
};