#pragma once
#include <SFML/System/Vector2.hpp>
#include "Component.hpp"
#include "Serializable.hpp"
#include "NetUtils.hpp"


namespace sl {

	/*Component that stores position of an entity. Can be extended in the future to include rotation, scale, etc.*/
	class TransformComponent: public sl::Component, public sl::Serializable {
	public:
		TransformComponent();
		TransformComponent(sf::Vector2f startPosition);
		TransformComponent(float x, float y);
		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);
		sf::Vector2f& getPosition();
		virtual void serialize(std::vector<uint8_t>& out) const override;
		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;
		virtual uint32_t getSerializeDataSize() const override;
		virtual uint32_t getDeserializeDataSize() const override;

		COMPONENT_TYPE(TransformComponent);
	private:
		sf::Vector2f position;
	};
}