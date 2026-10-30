#pragma once
#include <SFML/System/Vector2.hpp>
#include "SFML/System/Time.hpp"
#include "system/systems/Component.hpp"
#include "network/Serializable.hpp"
#include <string>

namespace sl {

	class MovementComponent: public sl::Component, public sl::Serializable {
	public:

		MovementComponent(float maxSpeed = 10, sf::Time maxAccelerationTime = sf::seconds(0.5f));

		/*Use this method if the direction and speed were obtained externally, such as throwing*/
		void addVelocityVector(sf::Vector2i direction, float speed);

		/*Use this method if the velocity vector were obtained externally, such as throwing*/
		void addVelocity(const sf::Vector2f& velocity);

		/*Use this method if the direction was obtained from within, such as player movement.*/
		void addDirection(sf::Vector2i direction);

		sf::Vector2f calculateDelta(float deltaTime);

		void braking(float deltaTime);

		//getters

		sf::Vector2f getVelocity() const { return velocityVector; }

		sf::Vector2i getVelocityDirection() const { return velocityDirectionVector; }

		sf::Vector2f getMovementEndPoint() const { return movementEndPoint; }

		float getMaxSpeed() const { return maxSpeed; }

		float getCurrentSpeed() const { return currentSpeed; }

		const sf::Time& getMaxAccelerationTime() const { return maxAccelerationTime; }

		const sf::Time& getCurrentAccelerationTime() const { return currentAccelerationTime; }

		//end getters

		//setters

		void setMovementEndPoint(sf::Vector2f movementEndPoint) { this->movementEndPoint = movementEndPoint; }

		void setVelocityDirection(const sf::Vector2i& direction);

		void setCurrentSpeed(float newCurrentSpeed) { currentSpeed = newCurrentSpeed; }

		void setCurrentAccelerationTime(const sf::Time& newAccelerationTime) { currentAccelerationTime = newAccelerationTime; }

		//end setters

		void resetVelocity() { velocityVector = sf::Vector2f(0.f, 0.f); }

		bool isMoving() const { return velocityDirectionVector != sf::Vector2i(0, 0); }

		virtual void serialize(std::vector<uint8_t>& out) const override;

		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;

		virtual uint32_t getSerializeDataSize() const override;

		virtual uint32_t getDeserializeDataSize() const override;

		bool isMovementAlreadyReseted = false;

		COMPONENT_TYPE(MovementComponent);
	private:
		sf::Vector2f velocityVector;
		sf::Vector2i velocityDirectionVector;
		sf::Vector2f movementEndPoint;
		float maxSpeed;
		float currentSpeed = 0.f;
		sf::Time maxAccelerationTime;
		sf::Time currentAccelerationTime;
		
		REGISTER_COMPONENT();
	};
}