#pragma once
#include <SFML/System/Vector2.hpp>
#include "SFML/System/Time.hpp"


class MovementComponent {
	public:
		MovementComponent(float maxSpeed, sf::Time maxAccelerationTime = sf::seconds(1.f));
		void addMovement(sf::Vector2i direction);
		void move(float deltaTime, sf::Vector2f& position);
		void addVelocity(const sf::Vector2f& velocity);
		sf::Vector2f getVelocity() const { return velocityVector; }
		void resetVelocity() { velocityVector = sf::Vector2f(0.f, 0.f); }
	private:
		sf::Vector2f velocityVector;
		float maxSpeed = 200.f;
		float CurrentSpeed = 0.f;
		sf::Time maxAccelerationTime;
		sf::Time currentAccelerationTime;
		sf::Vector2f asNormalized(const sf::Vector2f& vector) const;
		bool isAccelerating = false;
	
};