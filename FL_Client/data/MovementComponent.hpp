#pragma once
#include <SFML/System/Vector2.hpp>
#include "SFML/System/Time.hpp"


class MovementComponent {
	public:
		MovementComponent(float maxSpeed, sf::Time maxAccelerationTime = sf::seconds(0.5f));

		/*Use this method if the direction and speed were obtained externally, such as throwing*/
		void addVelocityVector(sf::Vector2i direction, float speed);

		/*Use this method if the velocity vector were obtained externally, such as throwing*/
		void addVelocity(const sf::Vector2f& velocity);

		/*Use this method if the direction was obtained from within, such as player movement.*/
		void addDirection(sf::Vector2i direction);

		sf::Vector2f move(float deltaTime, const sf::Vector2f& position);
		sf::Vector2f getVelocity() const { return velocityVector; }
		void resetVelocity() { velocityVector = sf::Vector2f(0.f, 0.f); }
	private:
		sf::Vector2f velocityVector;
		sf::Vector2i directionVector;
		float maxSpeed = 200.f;
		float CurrentSpeed;
		sf::Time maxAccelerationTime;
		sf::Time currentAccelerationTime;
		sf::Vector2f asNormalized(const sf::Vector2f& vector) const;
		sf::Vector2i asNormalized(const sf::Vector2i& vector) const;
		sf::Vector2i inBounds(const sf::Vector2i& vector, const sf::Vector2i& minBounds, const sf::Vector2i& maxBounds) const;
	
};