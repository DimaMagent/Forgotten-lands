#pragma once
#include <SFML/System/Vector2.hpp>
#include "SFML/System/Time.hpp"
#include "Component.hpp"
#include "Serializable.hpp"
#include <string>

// TODO: Entity не теряет скорость, если стоит на месте
namespace sl {

	class MovementComponent: public sl::Component, public sl::Serializable {
	public:

		MovementComponent(float maxSpeed = 0, sf::Time maxAccelerationTime = sf::seconds(0.5f));

		/*Use this method if the direction and speed were obtained externally, such as throwing*/
		void addVelocityVector(sf::Vector2i direction, float speed);

		/*Use this method if the velocity vector were obtained externally, such as throwing*/
		void addVelocity(const sf::Vector2f& velocity);

		/*Use this method if the direction was obtained from within, such as player movement.*/
		void addDirection(sf::Vector2i direction);

		sf::Vector2f move(float deltaTime, const sf::Vector2f& position);
		sf::Vector2f getVelocity() const { return velocityVector; }
		void resetVelocity() { velocityVector = sf::Vector2f(0.f, 0.f); }

		bool isMoving() const { return directionVector != sf::Vector2i(0, 0); }

		virtual void serialize(std::vector<uint8_t>& out) const override;
		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;
		virtual uint32_t getSerializeDataSize() const override;
		virtual uint32_t getDeserializeDataSize() const override;

		COMPONENT_TYPE(MovementComponent);
	private:
		sf::Vector2f velocityVector;
		sf::Vector2i directionVector;
		float maxSpeed;
		float currentSpeed = 0.f;
		sf::Time maxAccelerationTime;
		sf::Time currentAccelerationTime;
		
		sf::Vector2f asNormalized(const sf::Vector2f& vector) const;
		sf::Vector2i asNormalized(const sf::Vector2i& vector) const;
		sf::Vector2i inBounds(const sf::Vector2i& vector, const sf::Vector2i& minBounds, const sf::Vector2i& maxBounds) const;

	};
}