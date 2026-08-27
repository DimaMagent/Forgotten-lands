#pragma once
#include <SFML/System/Vector2.hpp>
#include "Component.hpp"
#include "Serializable.hpp"
#include "NetUtils.hpp"
#include "LockFreeDelegate.hpp"
#include "Cell.hpp"

namespace sl {

	class Entity;

	class TransformComponent: public sl::Component, public sl::Serializable {
	public:
		sl::LockFreeDelegate<sf::Vector2f> onCellChanged;

		TransformComponent();
		TransformComponent(sf::Vector2f startPosition);
		TransformComponent(float x, float y);

		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);
		void setRotation(int x, int y);
		void setRotation(const sf::Vector2i& rotation);

		sf::Vector2f getPosition() const { return position; }
		sf::Vector2i getRotation() const { return rotation; }

		virtual void serialize(std::vector<uint8_t>& out) const override;
		virtual bool deserialize(const std::vector<uint8_t>& out, size_t& offset) override;
		virtual uint32_t getSerializeDataSize() const override;
		virtual uint32_t getDeserializeDataSize() const override;

		COMPONENT_TYPE(TransformComponent);
	private:
		sf::Vector2f position;
		// as normal
		sf::Vector2i rotation;

		REGISTER_COMPONENT();
	};
}